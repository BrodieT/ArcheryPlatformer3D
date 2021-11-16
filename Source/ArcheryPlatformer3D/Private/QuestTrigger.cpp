// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestTrigger.h"

// Sets default values for this component's properties
UQuestTrigger::UQuestTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestTrigger::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		if (UPlayerQuestManager* questManager = (UPlayerQuestManager*)Player->GetComponentByClass(UPlayerQuestManager::StaticClass()))
		{
			_playerQuestManager = questManager;
		}
	}

	_isTriggered = false;
}


// Called every frame
void UQuestTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestTrigger::TriggerQuest()
{
	if (_isTriggered)
		return;

	if (_playerQuestManager != nullptr)
	{
		_playerQuestManager->CompleteObjective(_linkedObjective);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find Quest Manager"));
	}
}