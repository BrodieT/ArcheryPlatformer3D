// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StaticMesh.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "ArcheryArrow.generated.h"

//This class represents the arrows that can be fired by the player
UCLASS()
class ARCHERYPLATFORMER3D_API AArcheryArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArcheryArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//The mesh component for the arrow
	UStaticMeshComponent* _meshComponent;
	//Projectile movement component for arrow physics
	UProjectileMovementComponent* _projectileMovement;
	//Sphere collider for collision detection
	USphereComponent* _sphereCollider;
	//The damage amount received from attacker
	int _damageAmount = 10;
	//Level of the attacker
	int _playerLevel = 1;
	UPROPERTY(EditAnywhere)
		float _cleanupTime = 30.0f;

	//Function for detecting an overlap event
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	//Function for detecting a hit event
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	//Function for destroying self
	void Cleanup();

	
};
