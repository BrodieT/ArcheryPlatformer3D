// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerRespawn.h"

// Sets default values for this component's properties
UPlayerRespawn::UPlayerRespawn()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerRespawn::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerRespawn::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerRespawn::Respawn(AActor* Player)
{
	Player->SetActorLocation(_respawnLocation->GetActorLocation());
}
