// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueInterface.h"

void UDialogueInterface::UpdateChracterName(FString name)
{
	CharacterName->SetText(FText::FromString(*name));
}

void UDialogueInterface::UpdateSubtitles(FString text)
{
	Subtitles->SetText(FText::FromString(*text));
}
