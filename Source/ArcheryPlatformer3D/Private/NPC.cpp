// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>
#include "Components/AudioComponent.h"
#include "QuestGiver.h"

ANPC::ANPC()
{
	//Init interact prompt
	Prompt = "Talk to " + _characterName;
	//Find the UI widget BP
	static ConstructorHelpers::FClassFinder<UUserWidget>HUDWidget(TEXT("WidgetBlueprint'/Game/Blueprints/DialogueInterface.DialogueInterface_C'"));
	//Store the BP class
	if (HUDWidget.Class != nullptr)
	{
		_dialogueInterfaceClass = HUDWidget.Class;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find Widget Class"));
	}
}

void ANPC::BeginPlay()
{
	//Init interact prompt
	Prompt = "Talk to " + _characterName;	
}

void ANPC::Interact()
{
	Super::Interact();
	//Begins the dialogue encounter when the player interacts with this NPC
	BeginDialogue();
}

//Starts the dialogue encounter
void ANPC::BeginDialogue()
{
	//Stop if there are no dialogue beats
	if (_dialogue.Num() <= 0)
		return;
	//Init the current beat index to the first one
	_currentBeat = 0;

	//Create the UI widget and add to viewport
	if (_dialogueInterfaceClass != nullptr)
	{
		_dialogueInterface = CreateWidget<UDialogueInterface>(GetWorld(), _dialogueInterfaceClass);
		_dialogueInterface->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Widget Class is NULL"));
	}

	//Lock the player movement and display the first beat
	if (AArcheryPlatformer3DCharacter* player = (AArcheryPlatformer3DCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter())
	{
		player->_isInDialogue = true;
		player->LockPlayerMovement();
		if (_dialogueInterface != nullptr)
			_dialogueInterface->UpdateChracterName(_characterName);
		DisplayBeat();
	}
	else
	{
		//If the player cant be found then finish the encounter immediately
		FinishDialogue();
	}
}

//Displays the current beat on the UI
void ANPC::DisplayBeat()
{
	//Stop if the current beat is invalid
	if (_currentBeat < 0 || _currentBeat >= _dialogue.Num())
		return;
	
	//Update the subtitles
	if (_dialogueInterface != nullptr)
		_dialogueInterface->UpdateSubtitles(_dialogue[_currentBeat]._subtitle);
	
	//Setting the delay to a negative value will result in the beat having to be clicked through
	if (_dialogue[_currentBeat]._delayTime >= 0.0f)
	{
		//Init time manager params
		FTimerManager* timerManager = &GetWorld()->GetTimerManager();
		FTimerHandle timerhandle;
		//Get the delay time for the current beat
		float delay = _dialogue[_currentBeat]._delayTime;

		//If there is an audio clip associated with the beat
		//Then add the delay time to the clip length
		if (_dialogue[_currentBeat]._audio != nullptr)
		{
			delay = _dialogue[_currentBeat]._delayTime + _dialogue[_currentBeat]._audio->Duration;

			//Play the audio clip
			if (UAudioComponent* source = (UAudioComponent*)GetComponentByClass(UAudioComponent::StaticClass()))
			{
				source->SetSound(_dialogue[_currentBeat]._audio);
				source->Play();
			}
		}

		//Begin the timer for moving to the next beat automatically
		if (timerManager != nullptr)
		{
			timerManager->SetTimer(timerhandle, this, &ANPC::NextBeat, delay, true);
		}
	}
}

//Moves to the next dialogue beat
void ANPC::NextBeat()
{
	if (_dialogue[_currentBeat]._audio != nullptr)
	{
		//Stop any audio clip
		if (UAudioComponent* source = (UAudioComponent*)GetComponentByClass(UAudioComponent::StaticClass()))
		{
			source->Stop();
		}
	}
	//increment index
	_currentBeat++;
	//Cleanup timers
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	//If the new index exceeds the array length then finish the encounter
	//otherwise display the new beat
	if (_currentBeat >= _dialogue.Num())
		FinishDialogue();
	else
		DisplayBeat();
}

//Finishes the encounter
void ANPC::FinishDialogue()
{
	//Unlock the player 
	if (AArcheryPlatformer3DCharacter* player = (AArcheryPlatformer3DCharacter*)GetWorld()->GetFirstPlayerController()->GetCharacter())
	{
		player->_isInDialogue = false;
		player->UnlockPlayerMovement();
	}

	//Cleanup the UI
	if (_dialogueInterface != nullptr)
		_dialogueInterface->RemoveFromViewport();

	_dialogueInterface = nullptr;

	//Reset the current beat
	_currentBeat = 0;
	//Cleanup the timers
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

	if (UQuestGiver* giver = (UQuestGiver*)GetComponentByClass(UQuestGiver::StaticClass()))
	{
		giver->GiveQuest();
	}

}

