// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "DrawDebugHelpers.h"
#include "CustomTriggerBox.generated.h"

//This class represents a modular custom trigger box for handling custom operations on player enter
UCLASS()
class ARCHERYPLATFORMER3D_API ACustomTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
public:
	ACustomTriggerBox();

	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

	UPROPERTY(EditAnywhere)
		bool _drawDebugLines = false; //Draws the debug lines in game mode when true
	UPROPERTY(EditAnywhere)
		bool _infiniteTriggers = false;
private :
	bool _hasBeenTriggered = false; //Ensures it cant be triggered twice
};
