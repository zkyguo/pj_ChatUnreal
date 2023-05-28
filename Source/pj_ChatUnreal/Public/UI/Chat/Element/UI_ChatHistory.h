// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_Base.h"
#include <Components/CheckBox.h>
#include <Components/MultiLineEditableText.h>
#include "UI_ChatHistory.generated.h"



/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_ChatHistory : public UUI_Base
{
	GENERATED_BODY()

public :
	UUI_ChatHistory();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void SetClicked(bool isClicked);

	void SetTextContent(const FText& InText);
	FString GetTextContent() const;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCheckBox> CheckBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableText> HistoryName;

protected:
	UFUNCTION()
	void OnStatementChange(bool bIsChecked);
};
