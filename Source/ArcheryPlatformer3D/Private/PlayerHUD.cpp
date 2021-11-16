// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::SetArcheryCharge(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;

	ArcheryCharge->SetPercent(value);
}

void UPlayerHUD::SetHealthBar(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;

	Healthbar->SetPercent(value);
}

void UPlayerHUD::SetAmmoCount(int amount)
{
	AmmoCounter->SetText(FText::FromString(FString::Printf(TEXT("%i"), amount)));
}

void UPlayerHUD::UpdateXP(int level, float currentAmount)
{
	if (currentAmount > 1.0f)
		currentAmount = 1.0f;
	else if (currentAmount < 0.0f)
		currentAmount = 0.0f;

	XpBar->SetPercent(currentAmount);

	PlayerLevel->SetText(FText::FromString(FString::Printf(TEXT("%i"), level)));
}

void UPlayerHUD::ShowPrompt(FString prompt)
{
	InteractionPrompt->SetVisibility(ESlateVisibility::Visible);

	InteractionPrompt->SetText(FText::FromString(*prompt));
}

void UPlayerHUD::HidePrompt()
{
	InteractionPrompt->SetVisibility(ESlateVisibility::Hidden);

}



void UPlayerHUD::UpdateCurrentObjective(FString objective)
{
	CurrentObjective->SetText(FText::FromString(objective));
}

void UPlayerHUD::UpdateCoins(int amount)
{
	CoinCounter->SetText(FText::FromString(FString::Printf(TEXT("%i"), amount)));
}
