// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GameFramework/Pawn.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "UObject/UObjectGlobals.h"
#include "BT_FindRandomLocation.generated.h"

//This class is a task for going to a random location within a set radius that can be carried out by the enemy AI
UCLASS()
class ARCHERYPLATFORMER3D_API UBT_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	//Constructor used to init node name
	UBT_FindRandomLocation(FObjectInitializer const& object_initializer);
	//Called when the task is executed
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory);
private:
	//The radius around the enemy that the random location will be chosen in
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float _searchRadius = 1500.0f;
};
