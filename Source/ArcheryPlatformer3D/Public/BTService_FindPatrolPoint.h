// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERYPLATFORMER3D_API UBTService_FindPatrolPoint : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_FindPatrolPoint();

	//Update next tick interval
	//this func should be considered as const (don't modify state of object) if node is not instanced
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
