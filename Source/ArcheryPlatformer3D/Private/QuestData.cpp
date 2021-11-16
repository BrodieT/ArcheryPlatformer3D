// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestData.h"
#include <ArcheryPlatformer3D/ArcheryPlatformer3DCharacter.h>

UQuestData::UQuestData()
{
}

UQuestData::~UQuestData()
{
}

void UQuestData::InitialiseQuest(UWorld* world)
{
	_isCompleted = false;
	_currentObjective = 0;

	for (int i = 0; i < _questStages.Num(); i++)
	{
		_questStages[i].GetDefaultObject()->_currentPart = 0;
		_questStages[i].GetDefaultObject()->_isComplete = false;
	}

	_world = world;
}

bool UQuestData::ContainsObjective(TSubclassOf<UQuestObjective> objective)
{
	return _questStages.Contains(objective);
}

void UQuestData::CompleteObjective(TSubclassOf<UQuestObjective> objective)
{
	//If the objective is not present in this quest then return
	if (!ContainsObjective(objective))
		return;

	int32 targetIndex = 0;

	if (_questStages.Find(objective, targetIndex))
	{
		_questStages[targetIndex].GetDefaultObject()->_currentPart++;

		if (_questStages[targetIndex].GetDefaultObject()->_currentPart >= _questStages[targetIndex].GetDefaultObject()->_numberOfParts)
		{
			FString output = "Completed Objective: " + _questStages[targetIndex].GetDefaultObject()->_objectiveDescription;
			UE_LOG(LogTemp, Warning, TEXT("%s"), *output);

			_questStages[targetIndex].GetDefaultObject()->_isComplete = true;
			_currentObjective++;
		}
		else
		{
			FString output = "Progressed Objective: " + _questStages[targetIndex].GetDefaultObject()->_objectiveDescription + FString::Printf(TEXT("(%i/%i)"), _questStages[targetIndex].GetDefaultObject()->_currentPart, _questStages[targetIndex].GetDefaultObject()->_numberOfParts);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *output);
		}

		if (_currentObjective >= _questStages.Num())
		{
			CompleteQuest();
			return;
		}
		else
		{
			if (AArcheryPlatformer3DCharacter* Player = (AArcheryPlatformer3DCharacter*)_world->GetFirstPlayerController()->GetCharacter())
			{
				if (Player->_playerHUD != nullptr)
				{

					int current = _currentObjective;
					FString desc = _questStages[current].GetDefaultObject()->_objectiveDescription;

					int numberOfParts = _questStages[current].GetDefaultObject()->_numberOfParts;

					if (numberOfParts > 1)
					{
						int currentPart = _questStages[current].GetDefaultObject()->_currentPart;

						if (currentPart >= numberOfParts)
							currentPart = numberOfParts;

						desc = _questStages[current].GetDefaultObject()->_objectiveDescription + FString::Printf(TEXT("(%i / %i)"), currentPart, numberOfParts);
					}



					Player->_playerHUD->UpdateCurrentObjective(desc);
				}
			}
		}
	}
}

void UQuestData::SetActive(bool active)
{
	_isActive = active;
}

void UQuestData::CompleteQuest()
{
	FString output = "Completed Quest: " + _questName;
	UE_LOG(LogTemp, Warning, TEXT("%s"), *output);

	if (_world != nullptr)
	{
		if (_world->GetFirstPlayerController() != nullptr)
		{

			if (AArcheryPlatformer3DCharacter* Player = (AArcheryPlatformer3DCharacter*)_world->GetFirstPlayerController()->GetCharacter())
			{
				if (Player->_playerHUD != nullptr)
				{
					Player->_playerHUD->UpdateCurrentObjective("");
				}

				for (int i = 0; i < _questRewards.Num(); i++)
				{
					switch (_questRewards[i]._type)
					{
					case HP:
						Player->AddHealth(_questRewards[i]._amount);
						break;
					case Xp:
						Player->AddXP(_questRewards[i]._amount);
						break;
					case Ammo:
						Player->AddAmmo(_questRewards[i]._amount);
						break;
					case Coin:
						Player->AddCoin(_questRewards[i]._amount);
						break;
					default:
						break;
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Player Controller is NULL"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("World is NULL"));
	}


	_isCompleted = true;
	_isActive = false;
}
