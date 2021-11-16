// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "BowChargeWidget.generated.h"

//This class is the UI widget class for the bow charge
UCLASS()
class ARCHERYPLATFORMER3D_API UBowChargeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Progress bar for representing the bow charge
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* _chargeAmount;


};
