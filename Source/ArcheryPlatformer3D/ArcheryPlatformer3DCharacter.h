// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerHUD.h"
#include "CombatModule.h"
#include "Blueprint/UserWidget.h"
#include "Interactable.h"
#include "ArcheryPlatformer3DCharacter.generated.h"

UCLASS(config=Game)
class AArcheryPlatformer3DCharacter : public ACharacter
{
	GENERATED_BODY()

	//The spring arm used to position the camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	//The camera that will follow the player character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* FollowCamera;
public:
	AArcheryPlatformer3DCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
		float BaseLookUpRate;
protected:
	void MoveForward(float Value); //Used for moving forward/back
	void MoveRight(float Value); //Used for moving left/right
	void LookYaw(float value); //Used for looking left/right
	void LookPitch(float value); //Used for looking up/down
	void FireButtonPressed(); //Called when the RMB is pressed
	void FireButtonReleased(); //Called when the RMB is released
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; //Used to bing input actions to functions
	void Interact(); //This function is called when the Interact button is pressed
public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; } //Camera Boom getter
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; } //Follow Camera getter


	////////////////////////////////////////////////////////
	//Below is where any added functionality will be placed
public:

	void BeginPlay() override; //Called at the start of the game
	void Tick(float deltaTime) override; //Called every frame

	
	void AddHealth(int value); //This function adds health to the player
	void SubtractHealth(int value); //This function subtracts health from the player
	void AddXP(int value); //This function adds XP to the player
	void LevelUp(); //This function is called when the XP exceeds the maximum and levels up the player
	void AddAmmo(int amount); //This gives the player more arrows for their bow
	void InteractionRaycast(); //This function carries out the raycast every frame for interactions
	void LockPlayerMovement(); //This locks the player movement
	void UnlockPlayerMovement(); //This unlocks the player movement
	void AddCoin(int amount); //Increases the player's money
	UFUNCTION()
		void UpdateUI(); //This updates the player HUD whenever a change is made


	TSubclassOf<class UUserWidget> _playerHUDClass; //Stores the BP class for the player UI
	class UPlayerHUD* _playerHUD; //The player UI widget in the viewport

	AInteractable* _hitInteractable; //The actor hit by the interactions raycast

	UCombatModule* _playerCombatModule; //The actor component that handles the player combat

	bool _isControlling = true; //Tracks whether the players movement is locked
	bool _isInDialogue = false; //Tracks whether the player is currently in dialogue
	
	
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _currentHealth; //The amount of health the player has
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _maxHealth; //The maximum amount of health the player can have. Increases with level ups
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _currentXP; //The amount of progress through the current XP level.
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _currentMaxXP; //The amount of XP needed to level up. Increases with level ups.
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _currentLevel; //The current player level
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		bool _isDead; //Tracks whether the player is dead. Used to prevent updates after death.
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _minHealth; //The min health value. Used when calculating max health after level up.
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _minXP; //The min XP value. Used when calculating max xp after level up.
	UPROPERTY(EditAnywhere, Category = Interaction)
		float _interactionDistance = 1000.0f; //The range on the interaction raycast
	UPROPERTY(BlueprintReadOnly, Category = PlayerStats)
		int _currentMoney; //The amount of health the player has

};

