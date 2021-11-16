// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create the mesh for this object
	_mesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");

	//Create rotator component
	_rotatorComponent = CreateDefaultSubobject<URotatingMovementComponent>("Rotator");
	
	//Set the newly created mesh component as the object root
	RootComponent = _mesh;

	//Get the Mesh asset file to be attached to the static mesh component
	auto MeshAsset = static ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	//Bind the asset to the mesh component if successfully found
	if (MeshAsset.Object != nullptr)
	{
		_mesh->SetStaticMesh(MeshAsset.Object);
	}

	//Set this object to collide with all dynamic objects (i.e. the player)
	_mesh->SetCollisionProfileName(TEXT("Pickup"));
	//Tell the rotator component to rotate the pickup
	_rotatorComponent->RotationRate = FRotator(10, 0, 10);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	//Get the player character
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	//Check the overlapping actor to see if it is the player
	if (OtherActor == Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Picked Up"));
		AArcheryPlatformer3DCharacter* playerChar =	(AArcheryPlatformer3DCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		if (playerChar != nullptr)
		{
			switch (_pickupType)
			{
			case HP:
				playerChar->AddHealth(_amount);
				break;
			case Ammo:
				playerChar->AddAmmo(_amount);
				break;
			case Xp:
				playerChar->AddXP(_amount);
				break;
			case Coin:
				playerChar->AddCoin(_amount);
				break;
			default:
				break;
			}
		}


	//Execute the picked up event
	OnPickedUp.ExecuteIfBound();
	}
}

