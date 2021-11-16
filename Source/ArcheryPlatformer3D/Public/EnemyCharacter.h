// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PickupSpawner.h"
#include "Blueprint/UserWidget.h"
#include "EnemyUI.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>
#include "Perception/PawnSensingComponent.h"
#include "EnemyCharacter.generated.h"


//The different types of alert status
UENUM()
enum AlertStatus
{
	Idle,
	Suspicious,
	Alert,
};


DECLARE_EVENT(AEnemyCharacter, FStateChangeSignature)
DECLARE_EVENT(AEnemyCharacter, FDeathSignature)

//This class represents the enemy character
UCLASS()
class ARCHERYPLATFORMER3D_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ResetCooldown(); //Resets the attack cooldown
	bool DetectPlayer(); //Checks if the enemy can see the player
	void OnDeath(); //Handles the death of this enemy
	UFUNCTION()
		void ReceiveDamage(int damage, int playerLevel); //Function for reducing the enemy health
	UFUNCTION()
		void Attack(); //Function for inflicting damage to the player
	UFUNCTION()
		void SetAlertStatus(TEnumAsByte<AlertStatus> status); //Function for setting the alert status
	UFUNCTION()
		void IncrementPatrolPoints(); //Moves onto the next patrol point in the list
	void Init();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FStateChangeSignature OnStateChange; //Event thats invoked whenever the alert status changes
	FDeathSignature OnDeathEvent;

	UPROPERTY(EditAnywhere, Category = Behaviour)
		class UBehaviorTree* _enemyBehaviourTree; //Behaviour tree for the enemy

	UPROPERTY(EditAnywhere)
		int _level = 1; //The enemy level
	int _currentHP = 100; //The current health value
	int _maxHP = 100; //The maximum health value
	int _damage = 10; //The default damage amount

	UPROPERTY(EditAnywhere)
		UPawnSensingComponent* _pawnSensor; //Used for sensing the player

	FTimerHandle _stateChangeTimerHandle; //Timer handle for moving between alert states 
	FTimerHandle _attackCooldownHandle; //Timer handle for the attack cooldown

	FTimerDelegate _stateChangeTimerDelegate; //Delegate for calling a parameter function upon completion of the state change timer
	FTimerManager* _timerManager; //Local store of the timer manager

	UPROPERTY(EditAnywhere, Category = "AI Logic")
		float _suspicionTime = 30.0f; //How long before exiting the suspicion state
	UPROPERTY(EditAnywhere, Category = "AI Logic")
		float _attackCooldown = 5.0f; //How long between attacks
	bool _canAttack = true; //Tracks whether the attack cooldown is finished
	
	AArcheryPlatformer3DCharacter* Player; //Local store of the player

	UWidgetComponent* _enemyUIComponent; //Enemy UI component for worldspace UI
	class UEnemyUI* _enemyUI; //The enemy UI

	//AI Logic Variables

	UPROPERTY(EditAnywhere, Category = "AI Logic")
		int _currentPatrolPoint = 0; //Which patrol point in the list is the current one
	UPROPERTY(EditAnywhere, Category = "AI Logic")
		TArray<AActor*> _patrolPoints; //The array of patrol points
	UPROPERTY(EditAnywhere, Category = "AI Logic") 
		TEnumAsByte<AlertStatus> _alertStatus = Idle; //Current alert status
	UPROPERTY(EditAnywhere, Category = "AI Logic")
		float _detectionRadius = 500.0f; //How close the player must be for the enemy to detect them (possibly deprecated)
};
