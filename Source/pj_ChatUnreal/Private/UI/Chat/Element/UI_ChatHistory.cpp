// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/Element/UI_ChatHistory.h"

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

}
