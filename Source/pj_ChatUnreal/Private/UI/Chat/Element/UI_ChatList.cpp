// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_ChatList.h"

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
}

void UUI_ChatList::OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod)
{
	OnSend();


	TextInput->SetText(FText());
}