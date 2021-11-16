// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArcheryPlatformer3DCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "NPC.h"
#include "PlayerQuestManager.h"
//////////////////////////////////////////////////////////////////////////
// AArcheryPlatformer3DCharacter

AArcheryPlatformer3DCharacter::AArcheryPlatformer3DCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	//Find the Player HUD Blueprint class
	static ConstructorHelpers::FClassFinder<UUserWidget>HUDWidget(TEXT("WidgetBlueprint'/Game/Blueprints/PlayerHUDBP.PlayerHUDBP_C'"));

	//Store the class found by the class finder if there is one
	if (HUDWidget.Class != nullptr)
	{
		_playerHUDClass = HUDWidget.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find Widget Class"));
	}
}

//This function is used for binding/detecting player input
void AArcheryPlatformer3DCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//Space controls jump
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//WASD controls character movement
	PlayerInputComponent->BindAxis("MoveForward", this, &AArcheryPlatformer3DCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArcheryPlatformer3DCharacter::MoveRight);
	//Mouse controls camera movement
	PlayerInputComponent->BindAxis("Turn", this, &AArcheryPlatformer3DCharacter::LookYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &AArcheryPlatformer3DCharacter::LookPitch);
	//Fire Button Controls
	PlayerInputComponent->BindAction("Fire1", IE_Pressed, this, &AArcheryPlatformer3DCharacter::FireButtonPressed);
	PlayerInputComponent->BindAction("Fire1", IE_Released, this, &AArcheryPlatformer3DCharacter::FireButtonReleased);
	//Interaction Controls
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AArcheryPlatformer3DCharacter::Interact);
	//Add binding for quest journal toggle, assuming there is a quest manager on the player
	if (UPlayerQuestManager* qm = (UPlayerQuestManager*)GetComponentByClass(UPlayerQuestManager::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Journal Toggle Bound"));
		PlayerInputComponent->BindAction("ToggleJournal", IE_Pressed, qm, &UPlayerQuestManager::ToggleQuestJournal);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest Manager Component Not Found"));
	}
	
	////DebugOnly
	//PlayerInputComponent->BindAction("LevelUp", IE_Pressed, this, &AArcheryPlatformer3DCharacter::LevelUp);
}

//Called on startup
void AArcheryPlatformer3DCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Create the widget for the player HUD if the class was successfully found and add to viewport
	if (_playerHUDClass != nullptr)
	{
		_playerHUD = CreateWidget<UPlayerHUD>(GetWorld(), _playerHUDClass);
		_playerHUD->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Class is NULL"));
	}

	//Find the Combat module component and assign it the skeletal mesh for the player
	_playerCombatModule = (UCombatModule*)GetComponentByClass(UCombatModule::StaticClass());
	_playerCombatModule->_playerSkeleton = GetMesh();

	//Initilise player stats
	_currentHealth = 100;
	_maxHealth = 100;
	_currentLevel = 0;
	_currentMaxXP = 1000;
	_currentXP = 0;
	_minHealth = 100;
	_minXP = 1000;
	_isDead = false;
	//Trigger the first level up
	LevelUp();
	//Update the UI now that the stats have been initialised
	UpdateUI();
}

//Called every frame
void AArcheryPlatformer3DCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	//Turn the player to the camera direction when drawing the bow
	if (_playerCombatModule->_isFireButtonHeldDown)
		bUseControllerRotationYaw = true;
	else
		bUseControllerRotationYaw = false;

	//Perform the interaction raycast
	InteractionRaycast();
}

//This function moves the character forward/back
void AArcheryPlatformer3DCharacter::MoveForward(float Value)
{
	//Dont process movement if player is locked
	if (!_isControlling)
		return;

	if ((Controller != nullptr) && (Value != 0.0f) && !_isDead)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//This function moves the character left/right
void AArcheryPlatformer3DCharacter::MoveRight(float Value)
{
	//Dont process movement if player is locked
	if (!_isControlling)
		return;

	if ( (Controller != nullptr) && (Value != 0.0f) && !_isDead)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

//This function lets the player look right/left
void AArcheryPlatformer3DCharacter::LookYaw(float value)
{
	//Dont process movement if player is locked
	if (!_isControlling)
		return;

	AddControllerYawInput(value);
}

//This function lets the player look up/down
void AArcheryPlatformer3DCharacter::LookPitch(float value)
{
	//Dont process movement if player is locked
	if (!_isControlling)
		return;

	AddControllerPitchInput(value);
}

//This function is called when the fire button is pressed
void AArcheryPlatformer3DCharacter::FireButtonPressed()
{
	//If the player is engaged in dialogue
	//Progress to the next dialogue beat
	//Otherwise begin drawing the bow
	if (!_isControlling && _isInDialogue)
	{
		if (ANPC* npc = Cast<ANPC>(_hitInteractable))
		{
			npc->NextBeat();
		}
	}
	else if (_isControlling && !_isInDialogue)
	{
		_playerCombatModule->ToggleBowCharge(true);
	}
}

//This function is called when the fire button is released
void AArcheryPlatformer3DCharacter::FireButtonReleased()
{
	//If the player is not in dialogue
	//Fire the bow and reset the charge
	if (_isControlling && !_isInDialogue)
	{
		_playerCombatModule->FireBow();
		_playerCombatModule->ToggleBowCharge(false);
	}
}

//This function increases the player's health up to the max value
void AArcheryPlatformer3DCharacter::AddHealth(int value)
{
	//If the new health value exceeds the max health
	//Set HP = MaxHP, otherwise increase by value
	if (_currentHealth + value >= _maxHealth)
		_currentHealth = _maxHealth;
	else
		_currentHealth += value;
	//Update the UI
	_playerHUD->SetHealthBar((float)_currentHealth / (float)_maxHealth);
}

//This function decreases the players health.
void AArcheryPlatformer3DCharacter::SubtractHealth(int value)
{
	//If the health falls below zero then set to zero and die. Otherwise subtract the value as normal
	if (_currentHealth - value <= 0)
	{
		_currentHealth = 0;
		_isDead = true;
	}
	else
	{
		_currentHealth -= value;
	}
	//Update the UI
	_playerHUD->SetHealthBar((float)_currentHealth / (float)_maxHealth);
}

//This function increases the player's XP
void AArcheryPlatformer3DCharacter::AddXP(int value)
{
	//If the xp exceeds the current max value then level up. Otherwise increase as normal.
	if (_currentXP + value >= _currentMaxXP)
		LevelUp();
	else
		_currentXP += value;
	//Update the UI
	_playerHUD->UpdateXP(_currentLevel, (float)_currentXP / (float)_currentMaxXP);
}

//This function increases the players level, updating XP and health where appropriate.
void AArcheryPlatformer3DCharacter::LevelUp()
{
	//Increment level counter
	_currentLevel++;

	//Update the new max XP value and reset the current xp counter
	_currentMaxXP = _currentLevel * _minXP;
	_currentXP = 0;

	//Update the max health value and boost the current health to the max value
	_maxHealth = _currentLevel * _minHealth;
	_currentHealth = _maxHealth;
	//Update UI
	_playerHUD->UpdateXP(_currentLevel, (float)_currentXP / (float)_currentMaxXP);
}

//This function gives the player more arrows for the bow
void AArcheryPlatformer3DCharacter::AddAmmo(int amount)
{
	//Increase the ammo count and update the UI in the
	//combat module, assuming one was found
	if (_playerCombatModule != nullptr)
	{
		_playerCombatModule->_ammoCount += amount;
		_playerCombatModule->UpdateUI();
	}
}

//This function carries out the raycast for the interactions system
void AArcheryPlatformer3DCharacter::InteractionRaycast()
{
	//Stores of camera transform data
	FVector CamLocation;
	FRotator CamRotation;
	//Get the camera transform
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CamLocation, CamRotation);

	//Find the Eyes socket on the player's skeletal mesh
	FVector EyesLocation;
	if (GetMesh() != nullptr && GetMesh()->DoesSocketExist("Eyes"))
	{
		EyesLocation = GetMesh()->GetSocketLocation("Eyes");
	}

	//Calculate the start and end points of the raycast
	const FVector StartLocation = EyesLocation;
	//Direction uses the camera rotation to allow for up/down aiming
	const FVector Direction = CamRotation.Vector();
	const FVector EndLocation = StartLocation + (Direction * _interactionDistance);

	//Init the collision params
	FCollisionQueryParams TraceParams(FName(TEXT("InteractTrace")), true, NULL);
	TraceParams.bTraceComplex = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(GetOwner());
	TraceParams.AddIgnoredActor(this);

	//Init the hit result
	FHitResult Hit(ForceInit);

	//Perform the raycast and show the UI prompt if something is hit
	if (GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_WorldDynamic, TraceParams))
	{
		//Attempt to cast the hit actor to interactable to determine if the 
		//player can interact with it
		if (AInteractable* interactable = Cast<AInteractable>(Hit.Actor))
		{
			//Store the hit interactable for use on player input
			_hitInteractable = interactable;
			
			//Update the UI
			if (_playerHUD != nullptr)
				_playerHUD->ShowPrompt(_hitInteractable->Prompt);
		}
		else
		{
			//Set stored HitActor to null
			_hitInteractable = nullptr;

			//Hide UI
			if (_playerHUD != nullptr)
				_playerHUD->HidePrompt();
		}
	}
	else
	{
		//Set stored HitActor to null
		_hitInteractable = nullptr;

		//Hide UI
		if (_playerHUD != nullptr)
			_playerHUD->HidePrompt();
	}
}

//This function handles the interaction itself when the button is pressed
void AArcheryPlatformer3DCharacter::Interact()
{
	if (!_isControlling && _isInDialogue)
		return;

	//Call the interact function on the interactable if the raycast hit something
	if (_hitInteractable != nullptr)
		_hitInteractable->Interact();
}

//This function locks the player movement
void AArcheryPlatformer3DCharacter::LockPlayerMovement()
{
	_isControlling = false;
}

//This function unlocks the player movement
void AArcheryPlatformer3DCharacter::UnlockPlayerMovement()
{
	_isControlling = true;
}

//This function updates the entire player HUD
//Used less frequently as the components are typically updated individually 
//when they are changed to be more efficient
void AArcheryPlatformer3DCharacter::UpdateUI()
{
	_playerHUD->SetHealthBar((float)_currentHealth / (float)_maxHealth);
	_playerHUD->UpdateXP(_currentLevel, (float)_currentXP / (float)_currentMaxXP);
	_playerHUD->UpdateCoins(_currentMoney);
	_playerHUD->UpdateCurrentObjective("");
	_playerHUD->SetAmmoCount(0);
}

void AArcheryPlatformer3DCharacter::AddCoin(int amount)
{
	_currentMoney += amount;

	if (_playerHUD != nullptr)
		_playerHUD->UpdateCoins(_currentMoney);
}