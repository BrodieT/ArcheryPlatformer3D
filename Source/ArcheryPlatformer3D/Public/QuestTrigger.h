// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestObjective.h"
#include "PlayerQuestManager.h"
#include "QuestTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCHERYPLATFORMER3D_API UQuestTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestTrigger();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UQuestObjective> _linkedObjective;

	UPlayerQuestManager* _playerQuestManager;

	UFUNCTION(BlueprintCallable)
	void TriggerQuest();

	bool _isTriggered = false;
};
