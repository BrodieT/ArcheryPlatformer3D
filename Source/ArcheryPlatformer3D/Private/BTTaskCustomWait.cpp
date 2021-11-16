// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskCustomWait.h"
#include "Math/UnrealMathUtility.h"

UBTTaskCustomWait::UBTTaskCustomWait(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Random Wait");
}

EBTNodeResult::Type UBTTaskCustomWait::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Generate a random wait time and assign to the inherited variable
	WaitTime = FMath::RandRange(_minWaitTime, _maxWaitTime);
	//After assigning the new wait time, return the value of the parent function
	//as no further processing is required
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}