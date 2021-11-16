// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class ARCHERYPLATFORMER3D_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* ArcheryCharge;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Healthbar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* PlayerLevel;

	UPROPERTY(meta = (BindWidget))
		class UProgressBar* XpBar;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* AmmoCounter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentObjective;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CoinCounter;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* InteractionPrompt;

public :
	UFUNCTION()
		void SetArcheryCharge(float value);
	UFUNCTION()
		void SetHealthBar(float value);
	UFUNCTION()
		void SetAmmoCount(int amount);
	UFUNCTION()
		void UpdateXP(int level, float currentAmount);
	UFUNCTION()
		void UpdateCurrentObjective(FString objective);
	UFUNCTION()
		void UpdateCoins(int amount);
	UFUNCTION()
		void ShowPrompt(FString prompt);
	UFUNCTION()
		void HidePrompt();
};
