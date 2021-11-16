// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup.generated.h"

DECLARE_DELEGATE(FPickedupEventSignature)

UENUM()
enum PickupType
{
	HP UMETA(DisplayName = "Health"),
	Ammo UMETA(DisplayName = "Ammunition"),
	Xp UMETA(DisplayName = "Experience"),
	Coin UMETA(DisplayName = "Money"),
};

UCLASS()
class ARCHERYPLATFORMER3D_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//On Trigger Enter
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY()
		UStaticMeshComponent* _mesh; //The mesh componenet for this pickup

	UPROPERTY()
		URotatingMovementComponent* _rotatorComponent; 

	UPROPERTY(EditAnywhere)
		TEnumAsByte<PickupType> _pickupType;
	UPROPERTY(EditAnywhere)
		int _amount;

	FPickedupEventSignature OnPickedUp;


};
