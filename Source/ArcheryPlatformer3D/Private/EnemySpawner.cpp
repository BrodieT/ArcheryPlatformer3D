// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "QuestTrigger.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//Initialise the spawn location
	_spawnLocation = CreateDefaultSubobject<USceneComponent>("SpawnLocation");

	if (RootComponent == nullptr)
		SetRootComponent(_spawnLocation);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	SpawnEnemy();
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	UWorld* _world = GetWorld();

	if (_world != nullptr && EnemyBP != nullptr)
	{
		//Spawn a new pickup object
		_currentEnemy = _world->SpawnActor<AEnemyCharacter>(EnemyBP, _spawnLocation->GetComponentTransform());
		_currentEnemy->_level = _spawnedInLevel;
		_currentEnemy->Init();

		if (UQuestTrigger* trigger = (UQuestTrigger*)GetComponentByClass(UQuestTrigger::StaticClass()))
		{
			UQuestTrigger* enemyTrig = (UQuestTrigger*)_currentEnemy->AddComponentByClass(UQuestTrigger::StaticClass(), false, FTransform::Identity, false);

			enemyTrig->_linkedObjective = trigger->_linkedObjective;
			enemyTrig->_playerQuestManager = trigger->_playerQuestManager;
			enemyTrig->_isTriggered = trigger->_isTriggered;
		}
		_currentEnemy->_patrolPoints = _linkedPatrolPoints;
		_currentEnemy->OnDeathEvent.AddUObject(this, &AEnemySpawner::StartRespawn);
	}
}

void AEnemySpawner::StartRespawn()
{
	GetWorld()->GetTimerManager().SetTimer(_myTimer, this, &AEnemySpawner::SpawnEnemy, 10, false);
}