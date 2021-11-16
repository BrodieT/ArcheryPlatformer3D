// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiver.h"
#include "PlayerQuestManager.h"

// Sets default values for this component's properties
UQuestGiver::UQuestGiver()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestGiver::BeginPlay()
{
	Super::BeginPlay();
	_giveQuest = false;
}


// Called every frame
void UQuestGiver::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestGiver::GiveQuest()
{
	if (_giveQuest)
		return;

	if (UPlayerQuestManager* manager = (UPlayerQuestManager*)GetWorld()->GetFirstPlayerController()->GetPawn()->GetComponentByClass(UPlayerQuestManager::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Giving Quest"));

		manager->AddQuest(Quest);
		_giveQuest = true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Quest Giver cant find Manager"));
	}

}

