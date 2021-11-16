// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "EnemyAIController.generated.h"

//This class is the controller for the enemy characters
UCLASS()
class ARCHERYPLATFORMER3D_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
private:
	//AI component references
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* _behaviourTreeComponent; 

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* _behaviourTree;


	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBlackboardComponent* _blackboard;

public:
	AEnemyAIController(FObjectInitializer const& object_initializer);

	void BeginPlay() override;
	class UBlackboardComponent* get_blackboard() const;
	// Called when the controller possesses a Pawn/Character
	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
		void RestartLogic(); //This function is used to restart the behaviour tree to break out of existing tasks
};
