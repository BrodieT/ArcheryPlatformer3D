// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "QuestGiver.h"
#include "QuestTrigger.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create and initialise the pawn sensor
	_pawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(FName("Player Detection"));
	
	if (_pawnSensor != nullptr)
	{
		_pawnSensor->bOnlySensePlayers = true;
		_pawnSensor->SetPeripheralVisionAngle(45.0f);
		_pawnSensor->bSeePawns = true;
		_pawnSensor->bEnableSensingUpdates = true;
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Store the player and timer manager
	Player = (AArcheryPlatformer3DCharacter*)GetWorld()->GetFirstPlayerController()->GetPawn();
	_timerManager = &GetWorld()->GetTimerManager();


	//Find the enemy UI component
	TArray<UObject*> ObjectArray;
	GetDefaultSubobjects(ObjectArray);

	for (UObject* object : ObjectArray)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *object->GetName());

		if (object->GetName() == "Healthbar")
		{
			_enemyUIComponent = Cast<UWidgetComponent>(object);

			if(_enemyUIComponent != nullptr)
				_enemyUI = Cast<UEnemyUI>(_enemyUIComponent->GetUserWidgetObject());
			break;
		}
	}

	Init();
}

void AEnemyCharacter::Init()
{

	//Use the enemy level to scale the max health
	_maxHP = _level * 50;
	_currentHP = _maxHP;

	//Update the healthbar
	if (_enemyUI != nullptr)
		_enemyUI->UpdateHealth((float)_currentHP / (float)_maxHP);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the player is detected, immediately move to alert
	//Otherwise, if already alert, move to suspicious
	if (DetectPlayer())
	{
		SetAlertStatus(Alert);
	}
	else if(_alertStatus == Alert)
	{
		SetAlertStatus(Suspicious);
	}

	//If in the suspicious state and not already counting down
	//Begin timer for returning to idle
	if (_alertStatus == Suspicious)
	{
		if (_timerManager != nullptr)
		{
			if (!_timerManager->IsTimerActive(_stateChangeTimerHandle))
			{
				_stateChangeTimerDelegate.BindUFunction(this, FName("SetAlertStatus"), Idle);
				_timerManager->SetTimer(_stateChangeTimerHandle, _stateChangeTimerDelegate, _suspicionTime, false);
			}
		}
	}

	//Update the worldspace UI to always face the player camera
	if (_enemyUI != nullptr)
	{
		_enemyUIComponent->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(_enemyUIComponent->GetComponentLocation(), UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->GetCameraLocation()));
	}

}

//This function increments the current patrol point index, ensuring it wraps back to 0 when reaching the bounds of the patrol points array
void AEnemyCharacter::IncrementPatrolPoints()
{
	//If increment will cause overflow, reset back to 0, otherwise increment as normal
	if (_currentPatrolPoint + 1 >= _patrolPoints.Num())
		_currentPatrolPoint = 0;
	else
		_currentPatrolPoint++;
}

//This function is used to inflict damage on the enemy
void AEnemyCharacter::ReceiveDamage(int damage, int playerLevel)
{
	//Calculate the damage modifier based on the level difference between the player and this enemy
	// This will increase or decrease the resulting damage based on whether the player is a higher or lower level than the enemy
	// The player will deal more damage if the enemy is a lower level and less damage if they are a higher level
	// If both are the same level then no modifier is applied and the standard damage value is used

	//Storage for the damage value for safe modification
	int dmg = damage;

	//Divide the player level by the enemy level to get the multiplier
	//e.g. if the player is double the enemy level then they will deal double damage
	// if they are half the enemy level then they will deal half damage
	float modifier = (float)playerLevel / (float)_level;

	//Cast the incoming damage to a float and multiply by the modifier to get the resulting damage
	//Cast the result back to an int (quicker than rounding properly and lost data will be negligible)
	dmg = (int)((float)damage * modifier);

	//Subtract the final damage value from the current health
	_currentHP -= dmg;

	//Update the Healthbar UI if applicable
	if (_enemyUI != nullptr)
		_enemyUI->UpdateHealth((float)_currentHP / (float)_maxHP);

	//If the health reaches or falls below zero then the enemy is dead
	if (_currentHP <= 0)
		OnDeath();
}

//This function is called when the cooldown timer finishes
void AEnemyCharacter::ResetCooldown()
{
	_canAttack = true;
}

//This function is used to inflict damage on the player
void AEnemyCharacter::Attack()
{
	//Proceed if the cooldown is finished
	if (Player && _canAttack)
	{
		//Reset cooldown to prevent spam attacks
		_canAttack = false;

		if (_timerManager != nullptr)
		{
			if (!_timerManager->IsTimerActive(_attackCooldownHandle))
			{
				_timerManager->SetTimer(_attackCooldownHandle, this, &AEnemyCharacter::ResetCooldown, _attackCooldown, false);
			}
		}

		//Similar to the recieve damage calculation, however this time the enemy level is being divided by the player's
		//This results in the enemy inflicting damage in much the same way as the player inflicts it on the enemy
		float modifier = (float)_level / (float)Player->_currentLevel;

		//Calculate damage output using the modifier value
		int damageOutput = (int)((float)_damage * modifier);

		//Call the player's subtract health function which will handle any clamping/death checks
		Player->SubtractHealth(damageOutput);
		
		//Debug Only
		UE_LOG(LogTemp, Warning, TEXT("Dealing %i damage to player"), damageOutput);
	}
}

//This function updates the alert status 
void AEnemyCharacter::SetAlertStatus(TEnumAsByte<AlertStatus> status)
{
	//Only proceed if the value is different from the current state
	if (status != _alertStatus)
	{
		//Cleanup timers
		_stateChangeTimerDelegate.Unbind();
		_timerManager->ClearTimer(_stateChangeTimerHandle);
		//Update alert status
		_alertStatus = status;
		//Invoke event to update behaviour tree
		OnStateChange.Broadcast();
	}
}

//This function handles the death of this enemy
void AEnemyCharacter::OnDeath()
{
	//Clamp the health to 0 and update the healthbar one last time
	//This simply prevents any potential graphical issues with the healthbar
	_currentHP = 0;
	if (_enemyUI != nullptr)
		_enemyUI->UpdateHealth(0.0f);

	//Spawn a loot pickup at the enemy's death location before cleaning up the body
	FActorSpawnParameters spawnInfo;
	APickupSpawner* LootSpawner = GetWorld()->SpawnActor<APickupSpawner>(APickupSpawner::StaticClass(), GetTransform());

	//If the loot is spawned in successfully
	if (LootSpawner != nullptr)
	{
		//TODO Remove this?
		//Update the position and rotation
		LootSpawner->SetActorLocation(GetActorLocation());
		LootSpawner->SetActorRotation(GetActorRotation());

		//Init the pickup type and amount
		//TODO make these random
		//Possibly scale the amount with the enemy level as well?
		LootSpawner->_pickupType = HP;
		LootSpawner->_amount = 10;
	}


	if (Player)
	{
		//Give the player XP for killing the enemy, scaled with the level difference as with the damage intake
		//This results in the player getting more XP if the enemy is a greater challenge and less if they are easier to defeat
		int xpReward = 100;
		float modifier = (float)_level / (float)Player->_currentLevel;
		//Calculate the final result using the modifier
		xpReward = (int)((float)xpReward * modifier);
		//Debug only
		UE_LOG(LogTemp, Warning, TEXT("Player is receiving %i XP"), xpReward);
		//Add the Xp to the player
		Player->AddXP(xpReward);
	}

	//Give the quest to the player upon death if there is one to give
	if (UQuestGiver* questGiver = (UQuestGiver*)GetComponentByClass(UQuestGiver::StaticClass()))
	{
		questGiver->GiveQuest();
	}

	//Trigger the quest for the player if there is one to trigger
	if (UQuestTrigger* questTrigger = (UQuestTrigger*)GetComponentByClass(UQuestTrigger::StaticClass()))
	{
		questTrigger->TriggerQuest();
	}
	OnDeathEvent.Broadcast();

	//Destroy this object to remove the enemy from the scene
	Destroy();
}

//This function is used to detect the player character
bool AEnemyCharacter::DetectPlayer()
{
	if (Player && _pawnSensor)
	{
		//If the player is within range and the pawn sensor has a line of sight return true
		if ((FVector::Dist(GetActorLocation(), Player->GetActorLocation()) <= _detectionRadius) && _pawnSensor->HasLineOfSightTo(Player))
		{
			return true;
		}
	}

	return false;
}


