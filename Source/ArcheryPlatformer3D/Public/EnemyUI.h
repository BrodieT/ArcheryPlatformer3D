// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyUI.generated.h"

//UI for the enemy, spawned in worldspace
UCLASS()
class ARCHERYPLATFORMER3D_API UEnemyUI : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* Healthbar;

	UFUNCTION()
		void UpdateHealth(float value);
};
