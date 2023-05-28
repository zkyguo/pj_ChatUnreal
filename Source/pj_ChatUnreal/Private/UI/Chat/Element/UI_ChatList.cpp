// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_ChatList.h"
#include "Components/ScrollBoxSlot.h"
#include "UI/Chat/UI_ChatMain.h"

UUI_ChatList::UUI_ChatList()
{
}

void UUI_ChatList::NativeConstruct()
{
	Super::NativeConstruct();
	SendButton->OnClicked.AddDynamic(this, &UUI_ChatList::OnSend);
	TextInput->OnTextCommitted.AddDynamic(this, &UUI_ChatList::OnTextCommit);
}

void UUI_ChatList::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_ChatList::OnSend()
{
	FText InText = TextInput->GetText();

	if (UUI_Chat* Chat = AddRequestChat(1, InText))
	{
		Chat->SetTextContent(InText);
	}
	if (UUI_ChatMain* ChatMain = GetUserWidgetsClass<UUI_ChatMain>(UUI_ChatMain::StaticClass()))
	{
		ChatMain->OnSendRequest(1, InText);
	}
	
	
	TextInput->SetText(FText());

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
				ScrollBoxSlot->SetPadding(FMargin(90.f, 10.f, 10.f, 10.f));
				ScrollBoxSlot->SetHorizontalAlignment(HAlign_Left);

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
			if (UScrollBoxSlot* ScrollBoxSlot = Cast<UScrollBoxSlot>(ListBox->AddChild(Chat)))
			{
				ScrollBoxSlot->SetPadding(FMargin(10.f, 10.f, 90.f, 10.f));
				ScrollBoxSlot->SetHorizontalAlignment(HAlign_Right);
				return Chat;
			}
		}
	}

	return NULL;
}