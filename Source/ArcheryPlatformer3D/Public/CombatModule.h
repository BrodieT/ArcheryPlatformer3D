// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "ArcheryArrow.h"
#include "BowChargeWidget.h"
#include "DrawDebugHelpers.h"
#include "PlayerHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatModule.generated.h"

//This class handles the combat processing for the player
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ARCHERYPLATFORMER3D_API UCombatModule : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatModule();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION()
		void ChargeUpBow(float deltaTime); //this function charges up the bow shot
	UFUNCTION()
		void FireBow(); //This function spawns the arrow and fires it
	void ToggleBowCharge();
	void ToggleBowCharge(bool setCharge);	
	void GetHUD(); //Finds the HUD 
	void UpdateUI(); //Used to update the ui
	
	UPROPERTY(BlueprintReadOnly, Category = PlayerCombat)
		float _archeryCharge; //How charged the bow shot is
	UPROPERTY(EditAnywhere, Category = PlayerCombat)
		float _chargeIncrements = 1.0f; //How fast the bow will charge
	//The player skeleton is stored to find the spawn location
	USkeletalMeshComponent* _playerSkeleton; 
	//The UI class for displaying the charge widget and ammo counter
	class UPlayerHUD* _playerHUD;
	//The template class for the arrows being spawned in
	UPROPERTY(EditAnywhere, Category = UI)
		class TSubclassOf<AActor> _playerArrowClass;
	//Tracks when the fire button is held down 
	//to allow the bow to charge up on tick
	bool _isFireButtonHeldDown;
	//The scale applied to the charge amount determining the starting velocity of the arrow
	float _arrowVelocityScalingFactor;
	UPROPERTY(EditAnywhere, Category = PlayerCombat)
		int _damage = 10; //The damage being applied by the arrow
	int _ammoCount = 0; //The amount of arrows the player has
};
