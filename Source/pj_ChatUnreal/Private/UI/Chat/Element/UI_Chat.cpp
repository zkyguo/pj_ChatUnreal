// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_Chat.h"

UUI_Chat::UUI_Chat()
{
}

void UUI_Chat::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_Chat::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_Chat::SetTextContent(const FText& Chat)
{
	TextContent->SetText(Chat);
}
