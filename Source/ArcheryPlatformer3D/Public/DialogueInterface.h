// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DialogueInterface.generated.h"

//This class represents the UI for the dialogue
UCLASS()
class ARCHERYPLATFORMER3D_API UDialogueInterface : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CharacterName;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Subtitles;

	//Functions for updating the UI
	void UpdateChracterName(FString name);
	void UpdateSubtitles(FString text);
	
};
