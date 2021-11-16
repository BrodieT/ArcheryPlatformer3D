// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_AttackPlayer.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"


UBTTask_AttackPlayer::UBTTask_AttackPlayer(FObjectInitializer const& object_initializer)
{
	//Set the name that will appear in the tree graph
	NodeName = TEXT("Attack Player");
}

EBTNodeResult::Type UBTTask_AttackPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//Get the enemy's controller and blackboard component
	auto EnemyController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
	auto Blackboard = OwnerComp.GetBlackboardComponent();

	//Attempt to cast the controller's character to the enemy character class
	if (AEnemyCharacter* Enemy = (AEnemyCharacter*)EnemyController->GetCharacter())
	{
		//Attack the player
		Enemy->Attack();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}