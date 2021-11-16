// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "QuestData.h"
#include "QuestObjective.h"
#include "PlayerQuestManager.h"
#include "QuestListButton.h"
#include "ObjectiveListEntry.h"
#include "QuestJournalWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERYPLATFORMER3D_API UQuestJournalWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	TSubclassOf<UUserWidget> QuestEntryButtonBP;
	TSubclassOf<UUserWidget> ObjectiveEntryBP;

	UPROPERTY(meta = (BindWidget))
		UScrollBox* ObjectivesList;
	UPROPERTY(meta = (BindWidget))
		UScrollBox* QuestList;
	UPROPERTY(meta = (BindWidget))
		UVerticalBox* QuestBreakdown;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* QuestName;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* QuestDesc;

	TArray<TSubclassOf<UQuestData>> _allQuests;

	TArray<UQuestListButton*> ActiveQuests;
	TArray<UObjectiveListEntry*> SelectedObjectives;
	int _selectedQuest = 0;

	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void Initialise(TArray<TSubclassOf<UQuestData>> QuestData, TSubclassOf<UUserWidget> QuestBP, TSubclassOf<UUserWidget> ObjectiveBP);
	UFUNCTION()
		void UpdateSelectedQuest(int questNum);
};
