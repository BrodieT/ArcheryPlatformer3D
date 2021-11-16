// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "EnemyBlackboardKeys.h"

UBT_FindRandomLocation::UBT_FindRandomLocation(FObjectInitializer const& object_initializer)
{
    //Set the node name on the tree graph
    NodeName = TEXT("Go To Random Location");
}

EBTNodeResult::Type UBT_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* nodeMemory)
{
    //Get AI controller and its NPC
    AEnemyAIController* const cont = Cast<AEnemyAIController>(ownerComp.GetAIOwner());
    AEnemyCharacter* const npc = Cast<AEnemyCharacter>(cont->GetPawn());

    //Obtain NPC location to use as an origin location
    FVector origin = npc->GetActorLocation();
    FNavLocation location;

    //Get the navigation system and generate a random location on the NavMesh
    UNavigationSystemV1* const navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
   
    //If the found point is on the nav mesh
    if (navSystem->GetRandomPointInNavigableRadius(origin, _searchRadius, location, nullptr))
    {
        //Move to the target location
        FVector TargetLocation = location.Location;
        cont->MoveTo(TargetLocation);

        //If the enemy is within a suitable radius of the target location
        if (FVector::Dist(cont->GetPawn()->GetActorLocation(), TargetLocation) <= 50.0f)
        {
            //Finish with success
            FinishLatentTask(ownerComp, EBTNodeResult::Succeeded);
        }
    }

    return EBTNodeResult::Succeeded;
}

