// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowingCharacter.h"

// Sets default values
AFollowingCharacter::AFollowingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFollowingCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFollowingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Get the current location
	FVector startPos = GetActorLocation();

	//Get the player's location
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

	//Get the direction to move in
	FVector moveDirection = playerPos - startPos;
	moveDirection.Normalize();

	//Move the character in that direction
	SetActorLocation(startPos + moveDirection);
		 

}

// Called to bind functionality to input
void AFollowingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

