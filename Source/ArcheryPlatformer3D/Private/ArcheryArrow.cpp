// Fill out your copyright notice in the Description page of Project Settings.

#include "ArcheryArrow.h"
#include "EnemyCharacter.h"

// Sets default values
AArcheryArrow::AArcheryArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AArcheryArrow::BeginPlay()
{
	Super::BeginPlay();

	//Find the components attached to this object
	_sphereCollider = Cast<USphereComponent>(RootComponent);
	_projectileMovement = (UProjectileMovementComponent*)GetComponentByClass(UProjectileMovementComponent::StaticClass());
	_meshComponent = (UStaticMeshComponent*)GetComponentByClass(UStaticMeshComponent::StaticClass());

	//Add custom hit and overlap functions to colliders events
	if (_sphereCollider != nullptr)
	{
		_sphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AArcheryArrow::OnBeginOverlap);
		_sphereCollider->OnComponentHit.AddDynamic(this, &AArcheryArrow::OnHit);
	}

}

// Called every frame
void AArcheryArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Turn the arrow to face the movement direction
	FRotator newRote = FRotator(UKismetMathLibrary::MakeRotFromX(GetVelocity()).Pitch, GetActorRotation().Yaw, GetActorRotation().Roll);
	SetActorRotation(newRote);
}

//Detects an overlapping actor
void AArcheryArrow::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	//Attempt to cast to an enemy character
	AEnemyCharacter* OtherEnemy = dynamic_cast<AEnemyCharacter*>(OtherActor);
	//if an enemy was overlapped
	if (OtherEnemy)
	{
		//Apply damage
		OtherEnemy->ReceiveDamage(_damageAmount, _playerLevel);
		Cleanup();
	}
	else
	{
		//Cleanup arrow if the overlapped actor is not an enemy
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, this, &AArcheryArrow::Cleanup, _cleanupTime, true);
	}
}

//This function cleans up timers and destroys this object
void AArcheryArrow::Cleanup()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	Destroy();
}


void AArcheryArrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//Cleanup arrow if a hit event is called
	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AArcheryArrow::Cleanup, _cleanupTime, true);
}