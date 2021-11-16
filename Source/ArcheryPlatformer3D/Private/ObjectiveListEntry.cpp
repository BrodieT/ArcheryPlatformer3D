// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveListEntry.h"

void UObjectiveListEntry::UpdateObjective(FString text)
{
	ObjectiveDesc->SetText(FText::FromString(text));
}

void UObjectiveListEntry::SetCompleteStatus(bool complete)
{
	if (complete)
	{

	}
	else
	{

	}
}