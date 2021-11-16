// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToPlayer.h"
#include "EnemyAIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Math/UnrealMathUtility.h"

UBTTask_MoveToPlayer::UBTTask_MoveToPlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Move To Player");
}


EBTNodeResult::Type UBTTask_MoveToPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get the enemy controller and blackboard component
	auto EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	auto Blackboard = OwnerComp.GetBlackboardComponent();

	//Get the player pawn
	AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

	//If the player was successfully found 
	if (Player)
	{
		//Move towards the player actor, choosing a random stopping distance
		//could be extended to perform ranged/melee attacks based on chosen stopping distance
		EnemyController->MoveToActor(Player, FMath::RandRange(35.0f, 100.0f));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}