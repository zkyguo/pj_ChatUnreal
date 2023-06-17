// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_Chat.h"

#include "Components/WrapBoxSlot.h"

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

void UUI_Chat::AddImage(UTexture2D* InTexture2D)
{
	if(UImage* InImage = NewObject<UImage>(this, UImage::StaticClass()))
	{
		if(UWrapBoxSlot* InWrapBoxslot = Cast<UWrapBoxSlot>(ImageList->AddChild(InImage)))
		{
			InWrapBoxslot->SetPadding(FMargin(2.F));
		}

		InImage->SetBrushFromTexture(InTexture2D, true);
	}
}
