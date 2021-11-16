// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatModule.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>

// Sets default values for this component's properties
UCombatModule::UCombatModule()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//Find the Arrow class Blueprint
	static ConstructorHelpers::FClassFinder<AArcheryArrow>ArrowClass(TEXT("Blueprint'/Game/Blueprints/ArrowBP.ArrowBP_C'"));
	
	//If the class is found store it for when its spawned
	if (ArrowClass.Class != nullptr)
	{
		_playerArrowClass = ArrowClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find Arrow Class"));
	}

	//Set the scaling factor for the arrows
	_arrowVelocityScalingFactor = 3000;
}


// Called when the game starts
void UCombatModule::BeginPlay()
{
	Super::BeginPlay();
	//Initialise the bow charge
	_archeryCharge = 0.0f;
	//Get the player HUD from the player
	GetHUD();
}

void UCombatModule::UpdateUI()
{
	//Update the charge and ammo count components of the player UI widget
	if (_playerHUD != nullptr)
	{
		_playerHUD->SetArcheryCharge(_archeryCharge / 1.0f);
		_playerHUD->SetAmmoCount(_ammoCount);
	}
}

void UCombatModule::GetHUD()
{
	//Get the player HUD from the player character class
	if (AArcheryPlatformer3DCharacter* player = (AArcheryPlatformer3DCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		_playerHUD = player->_playerHUD;
	//Update the UI after it is found
	UpdateUI();
}

// Called every frame
void UCombatModule::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//If the fire button is held down then charge up the bow shot
	if (_isFireButtonHeldDown)
		ChargeUpBow(DeltaTime);
}

void UCombatModule::ToggleBowCharge()
{
	//Switches between the button held down states
	_isFireButtonHeldDown = !_isFireButtonHeldDown;
}


void UCombatModule::ToggleBowCharge(bool setCharge)
{
	//Sets the button held down state
	_isFireButtonHeldDown = setCharge;
}

void UCombatModule::ChargeUpBow(float deltaTime)
{
	//if there are arrows in the quiver
	if (_ammoCount > 0)
	{
		//Increase bow charge
		_archeryCharge += (_chargeIncrements * deltaTime);

		//Clamp the value between 0 and 1
		if (_archeryCharge >= 1.0f)
			_archeryCharge = 1.0f;

		//Find the HUD if its null
		if (_playerHUD == nullptr)
			GetHUD();

		//Update the UI
		if(_playerHUD != nullptr)
			_playerHUD->SetArcheryCharge(_archeryCharge / 1.0f);
	}
}

void UCombatModule::FireBow()
{
	//If there are arrows to fire
	if (_ammoCount > 0)
	{
		if (_playerArrowClass != nullptr)
		{
			//Create Arrow Object
			//Apply velocity and use charge to scale based on bow draw
			FActorSpawnParameters spawnInfo;
			AArcheryArrow* spawnedArrow = GetWorld()->SpawnActor<AArcheryArrow>(_playerArrowClass.Get());

			//If successfully spawned
			if (spawnedArrow != NULL)
			{
				//Decrement ammo counter
				_ammoCount--;
				//Use a default starting location of the players location
				FVector StartingVector = GetOwner()->GetActorLocation();
				//If the skeleton has been assigned and the spawn point socket is found
				if (_playerSkeleton != nullptr && _playerSkeleton->DoesSocketExist("BulletSocket"))
				{
					//Set the starting location to be the socket location
					StartingVector = _playerSkeleton->GetSocketLocation("BulletSocket");
				}

				//Stores of camera transform data
				FVector CamLocation;
				FRotator CamRotation;
				//Get the camera transform
				GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(CamLocation, CamRotation);

				//Set the position and rotation of the arrow
				spawnedArrow->SetActorLocation(StartingVector);
				spawnedArrow->SetActorRotation(CamRotation);
				//Assign the base damage amount and the player level for scaling
				spawnedArrow->_damageAmount = _damage;
				if(AArcheryPlatformer3DCharacter* player = (AArcheryPlatformer3DCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
					spawnedArrow->_playerLevel = player->_currentLevel;

				//Ensure the arrow has the projectile movement component
				if (spawnedArrow->_projectileMovement != nullptr)
				{
					//Calculate the velocity for the arrow
					FVector velocity = spawnedArrow->GetActorForwardVector() * (_arrowVelocityScalingFactor * _archeryCharge);
					//Calculate the maximum velocity for the arrow
					float maxSpeed = UKismetMathLibrary::Max(UKismetMathLibrary::Max(UKismetMathLibrary::Abs(velocity.X), UKismetMathLibrary::Abs(velocity.Y)), UKismetMathLibrary::Abs(spawnedArrow->_projectileMovement->MaxSpeed));
					//Assign the velocity and max velocity
					spawnedArrow->_projectileMovement->MaxSpeed = maxSpeed;
					spawnedArrow->_projectileMovement->Velocity = velocity;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Spawned Arrow is missing the projectile movement component"));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to spawn Arrow"));
			}
		}
	}

	//Reset the bow charge
	_archeryCharge = 0.0f;

	//Check the hud can be found
	if (_playerHUD == nullptr)
		GetHUD();
	//Update the UI
	UpdateUI();
}

