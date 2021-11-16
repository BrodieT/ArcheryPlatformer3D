// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestData.h"
#include "QuestObjective.h"
#include "PlayerQuestManager.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCHERYPLATFORMER3D_API UPlayerQuestManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerQuestManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
	TArray<TSubclassOf<UQuestData>> _questList;

	TSubclassOf<UUserWidget> _journalUIClass;
	TSubclassOf<UUserWidget> _objectiveUIClass;
	TSubclassOf<UUserWidget> QuestEntryBP;

	UUserWidget* _questJournal;

	void ToggleQuestJournal();

	UFUNCTION()
		void CompleteObjective(TSubclassOf<UQuestObjective> objective);
	UFUNCTION()
		void AddQuest(TSubclassOf<UQuestData> quest);
	UFUNCTION()
		bool IsAlreadyComplete(TSubclassOf<UQuestObjective> objective);
};
