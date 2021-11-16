// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestObjective.h"
#include "Pickup.h"
#include "QuestData.generated.h"


USTRUCT()
struct FQuestReward
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		TEnumAsByte<PickupType> _type; //The type of reward being given
	UPROPERTY(EditAnywhere)
		int _amount; //The amount of the reward type to be given
};

UCLASS(Blueprintable)
class ARCHERYPLATFORMER3D_API UQuestData : public UObject
{
	GENERATED_BODY()

public:
	UQuestData();
	~UQuestData();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString _questName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString _questDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<TSubclassOf<UQuestObjective>> _questStages;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int _currentObjective;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool _isActive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool _isCompleted;
	UPROPERTY(EditAnywhere)
		TArray<FQuestReward> _questRewards;

	UFUNCTION(BlueprintCallable)
		bool ContainsObjective(TSubclassOf<UQuestObjective> objective);
	UFUNCTION(BlueprintCallable)
		void CompleteObjective(TSubclassOf<UQuestObjective> objective);
	UFUNCTION(BlueprintCallable)
		void SetActive(bool active);
	UFUNCTION(BlueprintCallable)
		void CompleteQuest();

	UFUNCTION(BlueprintCallable)
		void InitialiseQuest(UWorld* world);

private :
	UWorld* _world;
};
