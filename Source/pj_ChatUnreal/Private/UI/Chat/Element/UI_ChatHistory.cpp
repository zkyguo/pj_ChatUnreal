// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_ChatHistory.h"
#include "ChatGameState.h"
#include <UI/Chat/UI_ChatHistoryList.h>
#include "UI/Chat/Element/UI_ChatList.h"

UUI_ChatHistory::UUI_ChatHistory()
{
}

void UUI_ChatHistory::NativeConstruct()
{
	Super::NativeConstruct();
	CheckBox->OnCheckStateChanged.AddDynamic(this, &UUI_ChatHistory::OnStatementChange);
}

void UUI_ChatHistory::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_ChatHistory::SetClicked(bool isClicked)
{
	CheckBox->SetCheckedState(isClicked ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
}

void UUI_ChatHistory::SetTextContent(const FText& InText)
{
	HistoryName->SetText(InText);
}

FString UUI_ChatHistory::GetTextContent() const
{
	return HistoryName->GetText().ToString();	
}

void UUI_ChatHistory::OnStatementChange(bool bIsChecked)
{
	if(AChatGameState *GameState = GetWorld()->GetGameState<AChatGameState>())
	{
		FString slotName = GetTextContent();
		GameState->SetCurrentSlotName(slotName);

		if(UUI_ChatHistoryList *HistoryList = GetUserWidgetsClass<UUI_ChatHistoryList>(UUI_ChatHistoryList::StaticClass()))
		{
			HistoryList->UpdateSelectHistorySlot(slotName);
		}
		//Update Chat history
		if (UUI_ChatList* ChatList = GetUserWidgetsClass<UUI_ChatList>(UUI_ChatList::StaticClass()))
		{
			ChatList->ClearChatWidget();
			ChatList->UpdateChatWidget();

		}
		
	}
}
