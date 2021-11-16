// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupSpawner.h"

// Sets default values
APickupSpawner::APickupSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initialise the spawn location
	_spawnLocation = CreateDefaultSubobject<USceneComponent>("SpawnLocation");

	if (RootComponent == nullptr)
		SetRootComponent(_spawnLocation);
}

// Called when the game starts or when spawned
void APickupSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnPickup();
}

// Called every frame
void APickupSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupSpawner::SpawnPickup()
{
	UWorld* _world = GetWorld();

	if (_world != nullptr)
	{
		//Spawn a new pickup object
		_currentPickup = _world->SpawnActor<APickup>(APickup::StaticClass(), _spawnLocation->GetComponentTransform());

		_currentPickup->_pickupType = _pickupType;
		_currentPickup->_amount = _amount;
		//Bind the Pickup collected function to the pickup's event
		_currentPickup->OnPickedUp.BindUObject(this, &APickupSpawner::PickupCollected);
	}
}

void APickupSpawner::PickupCollected()
{
	if (_canRespawn)
	{
		//Begin a timer for 10 seconds that will spawn a new pickup when it concludes
		GetWorld()->GetTimerManager().SetTimer(_myTimer, this, &APickupSpawner::SpawnPickup, 10, false);
	}

	//Unbind the event before destroying the object
	_currentPickup->OnPickedUp.Unbind();
	
	//Remove the object from the scene
	_currentPickup->Destroy();

	//Destroy the spawner if it is no longer needed
	if (!_canRespawn)
		Destroy();
}

