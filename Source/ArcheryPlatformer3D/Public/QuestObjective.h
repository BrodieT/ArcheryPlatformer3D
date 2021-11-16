// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "QuestObjective.generated.h"

UENUM()
enum QuestStageType
{
	Area UMETA(DisplayName = "Location-Based"),
	Combat UMETA(DisplayName = "Combat"),
	Dialogue UMETA(DisplayName = "Dialogue Choices"),
	Interaction UMETA(DisplayName = "Interaction-Based")
};


UCLASS(Blueprintable)
class ARCHERYPLATFORMER3D_API UQuestObjective : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = QuestData)
		FString _objectiveDescription;
	UPROPERTY(EditAnywhere, Category = QuestData)
		bool _isComplete = false;
	UPROPERTY(EditAnywhere, Category = QuestData)
		bool _isMultiPart = false;
	UPROPERTY(EditAnywhere, Category = QuestData)
		int _numberOfParts = 1;
	int _currentPart = 0;

};
