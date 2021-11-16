// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
//#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "EnemyBlackboardKeys.h"

UBTTask_FindPatrolPoint::UBTTask_FindPatrolPoint(FObjectInitializer const& object_initializer)
{
    NodeName = TEXT("Move To Patrol Location");
}

EBTNodeResult::Type UBTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    //Get AI controller and its NPC
    AEnemyAIController* const cont = Cast<AEnemyAIController>(ownerComp.GetAIOwner());
    AEnemyCharacter* const npc = Cast<AEnemyCharacter>(cont->GetPawn());

    //Get the navigation system and generate a random location on the NavMesh
    UNavigationSystemV1* const navSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    //Ensure the patrol points are valid
    if (npc->_currentPatrolPoint >= 0 && npc->_currentPatrolPoint < npc->_patrolPoints.Num() && npc->_patrolPoints.Num() > 0)
    {
        //Store the patrol point location and move towards it
        FVector TargetLocation = npc->_patrolPoints[npc->_currentPatrolPoint]->GetActorLocation();
        cont->MoveToActor(npc->_patrolPoints[npc->_currentPatrolPoint]);
        

       /* float dist = FVector::Dist(cont->GetPawn()->GetActorLocation(), TargetLocation);
        UE_LOG(LogTemp, Warning, TEXT("I am %f metres away"), dist);*/

      
        //if the enemy is within range of the target location
        if (FVector::Dist(cont->GetPawn()->GetActorLocation(), TargetLocation) <= 60.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("Arrived"));
            //Move to the next patrol point
            npc->IncrementPatrolPoints();
            //Finish with success
            FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
            return EBTNodeResult::Succeeded;

        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid"));

        FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
        return EBTNodeResult::Succeeded;
    }


    return EBTNodeResult::Failed;
}