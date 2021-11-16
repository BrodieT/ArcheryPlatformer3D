// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "PickupSpawner.generated.h"

UCLASS()
class ARCHERYPLATFORMER3D_API APickupSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickupSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SpawnPickup();

	UPROPERTY()
		APickup* _currentPickup;

	FTimerHandle _myTimer;

	UPROPERTY()
		USceneComponent* _spawnLocation;

	UFUNCTION()
		void PickupCollected();

	UPROPERTY(EditAnywhere)
		TEnumAsByte<PickupType> _pickupType;
	UPROPERTY(EditAnywhere)
		int _amount;

	UPROPERTY(EditAnywhere)
		bool _canRespawn = false;
};
