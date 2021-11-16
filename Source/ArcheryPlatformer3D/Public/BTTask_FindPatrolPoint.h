// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "BTTask_FindPatrolPoint.generated.h"

//This class is a task for going to the next patrol point that can be carried out by the enemy ai
UCLASS(Blueprintable)
class ARCHERYPLATFORMER3D_API UBTTask_FindPatrolPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_FindPatrolPoint(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
};
