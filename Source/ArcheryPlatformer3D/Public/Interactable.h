// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

//This is the base class for all interactables
UCLASS()
class ARCHERYPLATFORMER3D_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Interact(); //To be overridden with specific functionality based on interactable type

	UPROPERTY(EditAnywhere, Category = Interaction)
		FString Prompt = "Use"; //Prompt for when the player is looking at this interactable
};
