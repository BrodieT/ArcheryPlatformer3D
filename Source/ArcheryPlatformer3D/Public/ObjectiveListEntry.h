// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "ObjectiveListEntry.generated.h"

//This class represents the UI for the quest objectives
UCLASS()
class ARCHERYPLATFORMER3D_API UObjectiveListEntry : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ObjectiveDesc;

	UFUNCTION()
		void UpdateObjective(FString text);
	UFUNCTION()
		void SetCompleteStatus(bool complete); 
};
