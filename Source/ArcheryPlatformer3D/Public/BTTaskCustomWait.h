// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_Wait.h"
#include "BTTaskCustomWait.generated.h"

//This class is a custom wait task for generating a random wait time
UCLASS()
class ARCHERYPLATFORMER3D_API UBTTaskCustomWait : public UBTTask_Wait
{
	GENERATED_BODY()
public:
	UBTTaskCustomWait(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
		float _minWaitTime = 0.0f; //The minimum wait time possible 
	UPROPERTY(EditAnywhere)
		float _maxWaitTime = 5.0f; //The maximum wait time possible
};
