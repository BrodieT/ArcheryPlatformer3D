// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestJournalWidget.h"
#include "Blueprint/WidgetTree.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>

void UQuestJournalWidget::NativeConstruct()
{
	
}

void UQuestJournalWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UQuestJournalWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UQuestJournalWidget::Initialise(TArray<TSubclassOf<UQuestData>> QuestData, TSubclassOf<UUserWidget> QuestBP, TSubclassOf<UUserWidget> ObjectiveBP)
{
	//Store the UI BPs
	QuestEntryButtonBP = QuestBP;
	ObjectiveEntryBP = ObjectiveBP;


	for (int j = 0; j < ActiveQuests.Num(); j++)
	{
		ActiveQuests[j]->RemoveFromViewport();
	}
	ActiveQuests.Empty();

	//Access the quest list
	_allQuests.Empty();
	_allQuests = QuestData;

	//Instantiate the quest button BPs as children of the quest list
	//and add them to the array
	if (QuestEntryButtonBP != nullptr)
	{
		for (int i = 0; i < _allQuests.Num(); i++)
		{
			UQuestData* quest = (UQuestData*)(_allQuests[i]->GetDefaultObject());

			if (quest != nullptr)
			{
				FString output = FString::Printf(TEXT("%s"), *quest->_questName);
				UQuestListButton* questButton = WidgetTree->ConstructWidget<UQuestListButton>(QuestEntryButtonBP, FName(*output));

				if (questButton != nullptr)
				{
					questButton->UpdateButton(output);
					questButton->buttonNumb = i;

					if (!questButton->OnSelectQuest.IsBound())
					{
						questButton->OnSelectQuest.AddUObject(this, &UQuestJournalWidget::UpdateSelectedQuest);
					}


					QuestList->AddChild(questButton);
					ActiveQuests.Add(questButton);
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Unable to Cast to UQuestData"));
			}
		}
	}


	QuestEntryButtonBP = ObjectiveBP;
	_selectedQuest = 0;

	if(_allQuests.Num() > 0)
		UpdateSelectedQuest(0);
}

void UQuestJournalWidget::UpdateSelectedQuest(int questNum)
{

	for (int j = 0; j < SelectedObjectives.Num(); j++)
	{
		SelectedObjectives[j]->RemoveFromViewport();
	}
	SelectedObjectives.Empty();

	if (questNum >= 0 && questNum < _allQuests.Num() && ObjectiveEntryBP != nullptr)
	{
		if (UQuestData* quest = (UQuestData*)(_allQuests[questNum]->GetDefaultObject()))
		{
			QuestName->SetText(FText::FromString(quest->_questName));
			QuestDesc->SetText(FText::FromString(quest->_questDesc));

			//Loop through objectives

			int iterateAmount = quest->_currentObjective + 1;

			if (iterateAmount >= quest->_questStages.Num())
				iterateAmount = quest->_questStages.Num();
			
			for (int i = 0; i < iterateAmount; i++)
			{
				UObjectiveListEntry* objectiveButton = WidgetTree->ConstructWidget<UObjectiveListEntry>(ObjectiveEntryBP);
				if (objectiveButton != nullptr)
				{
					//Update widget text
					int partCount = quest->_questStages[i].GetDefaultObject()->_currentPart;

					if (partCount >= quest->_questStages[i].GetDefaultObject()->_numberOfParts)
						partCount = quest->_questStages[i].GetDefaultObject()->_numberOfParts;

					FString output = quest->_questStages[i].GetDefaultObject()->_objectiveDescription + FString::Printf(TEXT("(%i / %i)"), partCount, quest->_questStages[i].GetDefaultObject()->_numberOfParts);
					objectiveButton->UpdateObjective(output);
					objectiveButton->SetCompleteStatus(quest->_questStages[i].GetDefaultObject()->_isComplete);
					ObjectivesList->AddChild(objectiveButton);
					SelectedObjectives.Add(objectiveButton);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to Add Objectives"));
		return;
	}

	if (AArcheryPlatformer3DCharacter* Player = (AArcheryPlatformer3DCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter())
	{
		if (Player->_playerHUD != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Updating Current Objective"));
			int current = _allQuests[questNum].GetDefaultObject()->_currentObjective;

			if (current >= _allQuests[questNum].GetDefaultObject()->_questStages.Num())
				return;

			FString desc = _allQuests[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_objectiveDescription;

			int numberOfParts = _allQuests[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_numberOfParts;

			if (numberOfParts > 1)
			{
				int currentPart = _allQuests[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_currentPart;

				if (currentPart >= numberOfParts)
					currentPart = numberOfParts;

				desc = _allQuests[questNum].GetDefaultObject()->_questStages[current].GetDefaultObject()->_objectiveDescription + FString::Printf(TEXT("(%i / %i)"), currentPart, numberOfParts);
			}

			Player->_playerHUD->UpdateCurrentObjective(desc);
		}
	}
}
