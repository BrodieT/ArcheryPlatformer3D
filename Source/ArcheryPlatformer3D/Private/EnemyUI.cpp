// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUI.h"

void UEnemyUI::UpdateHealth(float value)
{
	if (value > 1.0f)
		value = 1.0f;
	else if (value < 0.0f)
		value = 0.0f;

	Healthbar->SetPercent(value);
}
