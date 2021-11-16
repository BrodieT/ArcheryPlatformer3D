// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "EnemyCharacter.h"
#include "CheckAlertStatus.generated.h"

//This task checks the alert status from the enemy character and compares it with the local stored comparator
UCLASS()
class ARCHERYPLATFORMER3D_API UCheckAlertStatus : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UCheckAlertStatus(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		TEnumAsByte<AlertStatus> _comparator = Idle; //The alert status being compared with
};
