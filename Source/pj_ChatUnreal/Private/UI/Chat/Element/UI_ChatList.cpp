// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_ChatList.h"
#include "ChatGameState.h"
#include "Components/ScrollBoxSlot.h"
#include "UI/Chat/UI_ChatMain.h"
#include "Sound/ChatSoundHandle.h"

UUI_ChatList::UUI_ChatList()
{
}

void UUI_ChatList::NativeConstruct()
{
	Super::NativeConstruct();
	SendButton->OnClicked.AddDynamic(this, &UUI_ChatList::OnSend);
	TextInput->OnTextCommitted.AddDynamic(this, &UUI_ChatList::OnTextCommit);
	//UpdateChatWidget();
}

void UUI_ChatList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_ChatList::OnSend()
{
	if (UUI_ChatMain* ChatMain = GetUserWidgetsClass<UUI_ChatMain>(UUI_ChatMain::StaticClass()))
	{
		if(ChatMain->IsNotInUse())
		{
			FText InText = TextInput->GetText();
			if (UUI_Chat* Chat = AddRequestChat(1, InText))
			{
				SubmitChat(1, InText);
			}
			ChatMain->OnSendRequest(1, InText);
			TextInput->SetText(FText());
		}

		
	}
}

void UUI_ChatList::OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	if(!Text.IsEmpty())
	{
		
		OnSend();
	}

}

UUI_Chat* UUI_ChatList::AddRequestChat(int32 ID, const FText& Content)
{
	if(ChatRightClass && !Content.IsEmpty())
	{
		if(UUI_Chat * Chat = CreateWidget<UUI_Chat>(this,ChatRightClass))
		{
			if(UScrollBoxSlot *ScrollBoxSlot = Cast<UScrollBoxSlot>(ListBox->AddChild(Chat)))
			{
				Chat->SetTextContent(Content);
				ScrollBoxSlot->SetPadding(10.f);
				ScrollBoxSlot->SetHorizontalAlignment(HAlign_Right);
				ListBox->ScrollToEnd();
				return Chat;
			}
		}
	}

	return NULL;
}

UUI_Chat* UUI_ChatList::AddResponseChat(int32 ID, const FText& Content)
{
	if (ChatLeftClass)
	{
		if (UUI_Chat* Chat = CreateWidget<UUI_Chat>(this, ChatLeftClass))
		{
			FString OriginString = Content.ToString();

			while (OriginString.StartsWith(TEXT("\n")))
			{
				OriginString.RemoveAt(0);
			}

			Chat->SetTextContent(FText::FromString(OriginString));
			if (UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ListBox->AddChild(Chat)))
			{
				ScrollBoxSlot->SetPadding(10.f);
				ScrollBoxSlot->SetHorizontalAlignment(HAlign_Left);
				ListBox->ScrollToEnd();
				return Chat;
			}
		}
	}

	return NULL;
}

void UUI_ChatList::SubmitChat(int32 ID, const FText& InContent)
{
	if(AChatGameState *GameState = GetWorld()->GetGameState<AChatGameState>())
	{
		GameState->AddText(ID, InContent.ToString());
	}


	//Display voice
	if(ID != 1)
	{
		ChatSoundHandle::SpeakFromText(ChatSoundHandle::ESoundHandleType::UE_Sound,
			TEXT(""),
			InContent.ToString());
	}
}

void UUI_ChatList::UpdateChatWidget()
{
	if (AChatGameState* GameState = GetWorld()->GetGameState<AChatGameState>())
	{
		if(UGameSaveData* currentData = GameState->FindCurrentGameSaveData())
		{
			for (auto &tmp : currentData->ChatDatas)
			{
				if(tmp.ID == 1)
				{
					AddRequestChat(tmp.ID, FText::FromString(tmp.InContent));
				}
				else
				{
					AddResponseChat(tmp.ID, FText::FromString(tmp.InContent));
				}
			}
		}
	}
}

void UUI_ChatList::ClearChatWidget()
{
	ListBox->ClearChildren();
}
