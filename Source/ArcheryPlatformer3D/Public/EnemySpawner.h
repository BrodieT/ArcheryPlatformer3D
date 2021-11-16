// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ArcheryPlatformer3D/Public/EnemyCharacter.h>
#include "EnemySpawner.generated.h"

UCLASS()
class ARCHERYPLATFORMER3D_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void SpawnEnemy();

	UPROPERTY(EditAnywhere)
		TSubclassOf<AEnemyCharacter> EnemyBP;

	AEnemyCharacter* _currentEnemy;
	UPROPERTY(EditAnywhere)
		TArray<AActor*> _linkedPatrolPoints;
	UPROPERTY(EditAnywhere)
		int _spawnedInLevel = 1;
	FTimerHandle _myTimer;
	UPROPERTY()
		USceneComponent* _spawnLocation;

	UFUNCTION()
		void StartRespawn();
	
};
