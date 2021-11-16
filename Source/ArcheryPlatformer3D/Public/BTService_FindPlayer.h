// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BTService_FindPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERYPLATFORMER3D_API UBTService_FindPlayer : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_FindPlayer();

	//Update next tick interval
	//this func should be considered as const (don't modify state of object) if node is not instanced
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
