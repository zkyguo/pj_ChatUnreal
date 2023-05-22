// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_Base.h"
#include "Element/UI_ChatList.h"
#include "UI_ChatHistoryList.h"
#include "UI_ChatMain.generated.h"



/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_ChatMain : public UUI_Base
{
	GENERATED_BODY()

	UUI_ChatMain();

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_ChatList> ChatList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_ChatHistoryList> ChatHistoryList;
};
