// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_Base.h"
#include <Components/Image.h>
#include <Components/MultiLineEditableText.h>
#include "UI_Chat.generated.h"



/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_Chat : public UUI_Base
{
	GENERATED_BODY()

	UUI_Chat();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UMultiLineEditableText> TextContent;

public:
	void SetTextContent(const FText& Chat);
};
