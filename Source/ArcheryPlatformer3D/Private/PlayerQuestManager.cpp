// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerQuestManager.h"
#include "QuestJournalWidget.h"
#include "Kismet/GameplayStatics.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>


// Sets default values for this component's properties
UPlayerQuestManager::UPlayerQuestManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	static ConstructorHelpers::FClassFinder<UUserWidget>JournalBP(TEXT("WidgetBlueprint'/Game/Blueprints/QuestJournal.QuestJournal_C'"));

	if (JournalBP.Class != nullptr)
	{
		_journalUIClass = JournalBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find JournalBP BP"));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>QuestButtonBP(TEXT("WidgetBlueprint'/Game/Blueprints/QuestListButtonBP.QuestListButtonBP_C'"));

	if (QuestButtonBP.Class != nullptr)
	{
		QuestEntryBP = QuestButtonBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find QuestButton BP"));
	}

	static ConstructorHelpers::FClassFinder<UUserWidget>ObjectiveBP(TEXT("WidgetBlueprint'/Game/Blueprints/ObjectiveListEntryBP.ObjectiveListEntryBP_C'"));

	if (ObjectiveBP.Class != nullptr)
	{
		_objectiveUIClass = ObjectiveBP.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find QuestButton BP"));
	}
}

void UPlayerQuestManager::ToggleQuestJournal()
{
	if (_questJournal != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Toggling OFF Journal"));

		_questJournal->RemoveFromViewport();
		_questJournal = nullptr;

		if (AArcheryPlatformer3DCharacter* Player = (AArcheryPlatformer3DCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			Player->_isControlling = true;
		}

		if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
		{
			Controller->bShowMouseCursor = false;
			Controller->bEnableClickEvents = false;
			Controller->bEnableMouseOverEvents = false;
		}
	}
	else
	{
		if (_journalUIClass != nullptr)
		{
			//_isControlling

			if (AArcheryPlatformer3DCharacter* Player = (AArcheryPlatformer3DCharacter*)UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
			{
				Player->_isControlling = false;
			}

			if (APlayerController* Controller = GetWorld()->GetFirstPlayerController())
			{
				Controller->bShowMouseCursor = true;
				Controller->bEnableClickEvents = true;
				Controller->bEnableMouseOverEvents = true;
			}

			//journal->Initialise();

			UQuestJournalWidget* journal = CreateWidget<UQuestJournalWidget>(GetWorld()->GetFirstPlayerController(), _journalUIClass);
			if (journal != nullptr)
			{
				journal->Initialise(_questList, QuestEntryBP, _objectiveUIClass);
				_questJournal = journal;
				if (_questJournal != nullptr)
					_questJournal->AddToViewport();
			}
			else
				UE_LOG(LogTemp, Warning, TEXT("Cast to Journal Unsuccessful"));



		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Journal UI Class NULL"));
		}
	}
}

// Called when the game starts
void UPlayerQuestManager::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPlayerQuestManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerQuestManager::CompleteObjective(TSubclassOf<UQuestObjective> objective)
{
	for (int i = 0; i < _questList.Num(); i++)
	{
		_questList[i].GetDefaultObject()->CompleteObjective(objective);
	}
}

void UPlayerQuestManager::AddQuest(TSubclassOf<UQuestData> quest)
{
	if (quest != nullptr)
	{
		_questList.Add(quest);
		_questList[_questList.Num() - 1].GetDefaultObject()->InitialiseQuest(GetWorld());

		int questNum = _questList.Num() - 1;
		if (AArcheryPlatformer3DCharacter* Player = (AArcheryPlatformer3DCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter())
		{
			if (Player->_playerHUD != nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Updating Current Objective"));
				int current = _questList[questNum].GetDefaultObject()->_currentObjective;
				FString desc = _questList[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_objectiveDescription;

				int numberOfParts = _questList[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_numberOfParts;

				if (numberOfParts > 1)
				{
					int currentPart = _questList[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_currentPart;

					if (currentPart >= numberOfParts)
						currentPart = numberOfParts;

					desc = _questList[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_objectiveDescription + FString::Printf(TEXT("(%i / %i)"), currentPart, numberOfParts);
				}

				Player->_playerHUD->UpdateCurrentObjective(desc);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to Add Quest"));
	}
}

bool UPlayerQuestManager::IsAlreadyComplete(TSubclassOf<UQuestObjective> objective)
{
	if (objective != nullptr)
	{
		//find quest that contains objective
		//if complete or current parts >= numb of parts
		//return true



	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Objective to Check"));
	}

	return false;
}

