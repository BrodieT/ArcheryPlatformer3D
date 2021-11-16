// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "QuestListButton.generated.h"

DECLARE_EVENT_OneParam(UQuestListButton, FSelectQuestDelegateSignature, int)

UCLASS()
class ARCHERYPLATFORMER3D_API UQuestListButton : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UButton* QuestButton;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* QuestName;

	UFUNCTION()
		void OnClick();

	int buttonNumb = 0;
	void NativeConstruct() override;
	void UpdateButton(FString text);

	FSelectQuestDelegateSignature OnSelectQuest;
};
