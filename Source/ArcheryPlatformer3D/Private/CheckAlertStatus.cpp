// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckAlertStatus.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "EnemyBlackboardKeys.h"

UCheckAlertStatus::UCheckAlertStatus(FObjectInitializer const& object_initializer)
{
    NodeName = TEXT("Check Alert Status");
}

EBTNodeResult::Type UCheckAlertStatus::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    //Get AI controller and its NPC
    AEnemyAIController* const cont = Cast<AEnemyAIController>(ownerComp.GetAIOwner());
    AEnemyCharacter* const npc = Cast<AEnemyCharacter>(cont->GetPawn());

    //Compare the comparator status with the enemy character's current status
    //Return successful if they match otherwise failed
    if (npc->_alertStatus == _comparator)
    {
        //Finish with success
        FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }
    else
    {
        //Finish with success
        FinishLatentTask(ownerComp, EBTNodeResult::Failed);
        return EBTNodeResult::Failed;
    }
}


