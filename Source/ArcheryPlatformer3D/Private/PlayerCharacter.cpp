// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "PlayerQuestManager.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialise capsule component using half width and height
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	//Prevent the character itself from rotating
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Make character rotate in direction of travel
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//Set rotation speed of character
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	//Set the jump velocity
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	//Set the amount of air control the player has
	GetCharacterMovement()->AirControl = 0.2f;

	//Create the camera boom
	_cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	_cameraBoom->SetupAttachment(RootComponent);
	_cameraBoom->TargetArmLength = 300.0f;
	_cameraBoom->bUsePawnControlRotation = true;

	//Create the camera object
	_followCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Follow Camera"));
	_followCamera->SetupAttachment(_cameraBoom, USpringArmComponent::SocketName);
	_followCamera->bUsePawnControlRotation = false;

	_isDead = false;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);

	if (UPlayerQuestManager* qm = (UPlayerQuestManager*)GetComponentByClass(UPlayerQuestManager::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Journal Toggle Bound"));
		PlayerInputComponent->BindAction("ToggleJournal", IE_Pressed, qm, &UPlayerQuestManager::ToggleQuestJournal);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest Manager Component Not Found"));
	}
}

void APlayerCharacter::MoveForward(float axis)
{
	if (!_isDead)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		//Get the forward direction of the yaw rotation
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		//Apply the movement to the character
		AddMovementInput(direction, axis);
	}
}

void APlayerCharacter::MoveRight(float axis)
{
	if (!_isDead)
	{
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation(0.0f, rotation.Yaw, 0.0f);

		//Get the right direction of the yaw rotation
		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		//Apply the movement to the character
		AddMovementInput(direction, axis);
	}
}

