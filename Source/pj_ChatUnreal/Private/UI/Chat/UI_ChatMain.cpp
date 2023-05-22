// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/UI_ChatMain.h"

UUI_ChatMain::UUI_ChatMain()
{
	isLocalLink = true;
}

void UUI_ChatMain::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI_ChatMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_ChatMain::OnSendRequest(int32 InID, const FText& inText)
{
	if(isLocalLink)
	{
		
	}
	else
	{
		
	}

}
