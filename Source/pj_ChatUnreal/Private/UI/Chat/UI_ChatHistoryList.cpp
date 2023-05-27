// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/UI_ChatHistoryList.h"
#include "ChatGameState.h"
#include "Components/ScrollBoxSlot.h"

UUI_ChatHistoryList::UUI_ChatHistoryList()
{
}

void UUI_ChatHistoryList::NativeConstruct()
{
	Super::NativeConstruct();

	AddSlotButton->OnClicked.AddDynamic(this, &UUI_ChatHistoryList::OnAddSlot);
	DeleteSlotButton->OnClicked.AddDynamic(this, &UUI_ChatHistoryList::OnDeleteSlot);
	UpdateHistorySlot();
	
}

void UUI_ChatHistoryList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

UUI_ChatHistory* UUI_ChatHistoryList::AddHistorySlot(const FString& InContent)
{
	if(ChatHistoryClass)
	{
		if(UUI_ChatHistory* InChatHistory = CreateWidget<UUI_ChatHistory>(this, ChatHistoryClass))
		{
			InChatHistory->SetTextContent(FText::FromString(InContent));
			if (UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ListBox->AddChild(InChatHistory)))
			{
				
			}

			return InChatHistory;
		}
	}

	return NULL;
}

void UUI_ChatHistoryList::UpdateHistorySlot()
{
	if (AChatGameState* GameState = GetWorld()->GetGameState<AChatGameState>())
	{
		if (auto* slotList = GameState->GetSaveSlotList())
		{
			for (FSaveSlot& tmp : slotList->Slots)
			{
				AddHistorySlot(tmp.SlotName.ToString());

			}
		}
	}
}

void UUI_ChatHistoryList::OnAddSlot()
{
	if(AChatGameState *GameState = GetWorld()->GetGameState<AChatGameState>())
	{
		FString SlotName = GameState->AddGameData();
		if(!SlotName.IsEmpty())
		{
			AddHistorySlot(SlotName);
		}

	}
}

void UUI_ChatHistoryList::OnDeleteSlot()
{
	if (AChatGameState* GameState = GetWorld()->GetGameState<AChatGameState>())
	{
		if(GameState->DeleteCurrentGameData())
		{
			ListBox->ClearChildren();
			UpdateHistorySlot();
		}
	}
}
