 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToPlayer.generated.h"

//This class is a task for moving to the player's location that can be carried out by the enemy ai
UCLASS()
class ARCHERYPLATFORMER3D_API UBTTask_MoveToPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_MoveToPlayer(FObjectInitializer const& object_initializer);
	//Starts the task, will either return Succeeded, InProgress, or Failed
	// (use FinishLatentTask() when returning InProgress)
	//this function should be considered as const (don't modify state of object) if node is not instanced!
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
