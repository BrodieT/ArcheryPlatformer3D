// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTriggerBox.h"
#include "QuestGiver.h"
#include "QuestTrigger.h"
#include "PlayerRespawn.h"


ACustomTriggerBox::ACustomTriggerBox()
{
	//Register Events
	OnActorBeginOverlap.AddDynamic(this, &ACustomTriggerBox::OnOverlapBegin);
}

void ACustomTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	//Draw debug lines if toggled
	if(_drawDebugLines)
		DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Purple, true, -1, 0, 5);
	//Reset trigger
	_hasBeenTriggered = false;
}

void ACustomTriggerBox::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		//If the player enters the trigger box for the first time
		if (OtherActor == GetWorld()->GetFirstPlayerController()->GetPawn() && !_hasBeenTriggered)
		{
			//Set triggered to true
			_hasBeenTriggered = true;

			//Check for quest giver component and give quest
			if (UQuestGiver* questGiver = (UQuestGiver*)GetComponentByClass(UQuestGiver::StaticClass()))
			{
				questGiver->GiveQuest();
			}

			//Check for quest trigger component and trigger quest
			if (UQuestTrigger* questTrigger = (UQuestTrigger*)GetComponentByClass(UQuestTrigger::StaticClass()))
			{
				questTrigger->TriggerQuest();
			}

			if (UPlayerRespawn* respawnCatcher = (UPlayerRespawn*)GetComponentByClass(UPlayerRespawn::StaticClass()))
			{
				respawnCatcher->Respawn(OtherActor);
			}

			if (_infiniteTriggers)
				_hasBeenTriggered = false;
		}
	}
}