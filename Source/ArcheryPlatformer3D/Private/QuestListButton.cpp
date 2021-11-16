// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListButton.h"

void UQuestListButton::OnClick()
{
	/*FString output = FString::Printf(TEXT("Button CLicked = %i"), buttonNumb);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *output);*/
	OnSelectQuest.Broadcast(buttonNumb);
}

void UQuestListButton::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (!QuestButton->OnClicked.IsBound())
	{
		QuestButton->OnClicked.AddDynamic(this, &UQuestListButton::OnClick);
	}
}

void UQuestListButton::UpdateButton(FString text)
{
	QuestName->SetText(FText::FromString(text));
}