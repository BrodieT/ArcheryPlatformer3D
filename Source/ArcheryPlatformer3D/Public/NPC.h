// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "DialogueInterface.h"
#include "NPC.generated.h"

//Storage for each dialogue beat
USTRUCT()
struct FDialogueBeat
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
		FString _subtitle; //Subtitle text to be displayed
	UPROPERTY(EditAnywhere)
		USoundCue* _audio; //Associated audio clip
	UPROPERTY(EditAnywhere)
		float _delayTime; //How long the beat should last for
};

//This class represents an NPC interactable, allowing the player to begin dialogue with them
UCLASS()
class ARCHERYPLATFORMER3D_API ANPC : public AInteractable
{
	GENERATED_BODY()
public:
	ANPC();
	virtual void BeginPlay() override;
	virtual void Interact() override;

	void BeginDialogue(); //Starts the dialogue encounter
	void DisplayBeat(); //Displays the current beat
	void NextBeat(); //Moves to the next beat
	void FinishDialogue(); //Finishes the encounter

	int _currentBeat = 0; //The current beat index
	UPROPERTY(EditAnywhere, Category = Interaction)
		FString _characterName = ""; //The name of the character to be displayed

	UPROPERTY(EditAnywhere, Category = Interaction)
		TArray<FDialogueBeat> _dialogue; //Array of the dialogue beats

	TSubclassOf<class UUserWidget> _dialogueInterfaceClass; //BP class for the dialogue UI
	class UDialogueInterface* _dialogueInterface; //The UI widget in the scene
};
