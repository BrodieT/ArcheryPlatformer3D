// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"


AEnemyAIController::AEnemyAIController(FObjectInitializer const& object_initializer)
{
    //Find, assign, and initialise the behaviour tree component & blackboard
    static ConstructorHelpers::FObjectFinder<UBehaviorTree> obj(TEXT("BehaviorTree'/Game/Blueprints/EnemyBT.EnemyBT'"));

    if (obj.Succeeded())
    {
        _behaviourTree = obj.Object;
    }

    _behaviourTreeComponent = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviourComponent"));
    _blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComponent"));
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    //Begin the behaviour tree logic
    RunBehaviorTree(_behaviourTree);
    _behaviourTreeComponent->StartTree(*_behaviourTree);
}

UBlackboardComponent* AEnemyAIController::get_blackboard() const
{
    return _blackboard;
}

// Called when the controller possesses a Pawn/Character
void AEnemyAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    //initialise the blackboard on possesion of the enemy
    if (_blackboard)
    {
        _blackboard->InitializeBlackboard(*_behaviourTree->BlackboardAsset);
    }

    //Convert InPawn to EnemyCharacter
    auto _character = Cast<AEnemyCharacter>(InPawn);

    //Check if pointers are valid
    if (_character && _character->_enemyBehaviourTree)
    {
        _blackboard->InitializeBlackboard(*_character->_enemyBehaviourTree->BlackboardAsset);
        //Subscribe to event to allow the enemy to break out of tasks when they change alert status
        _character->OnStateChange.AddUObject(this, &AEnemyAIController::RestartLogic);
    }
}

void AEnemyAIController::RestartLogic()
{
    _behaviourTreeComponent->RestartTree();
}