// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_Base.h"
#include "Element/UI_ChatHistory.h"
#include <Components/ScrollBox.h>
#include <Components/Button.h>
#include "UI_ChatHistoryList.generated.h"



/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_ChatHistoryList : public UUI_Base
{
	GENERATED_BODY()
public:
	enum EHandleHistorySlotResult
	{
		INPROGRESS,
		COMPLETE,
	};

	UUI_ChatHistoryList();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_ChatHistory> ChatHistoryClass;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ListBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> AddSlotButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> DeleteSlotButton;

	UUI_ChatHistory* AddHistorySlot(const FString& InContent);

	void UpdateHistorySlot();
	void UpdateSelectHistorySlot();
	void UpdateSelectHistorySlot(const FString &SlotName);

	void CallHistorySlot(TFunction<EHandleHistorySlotResult(UUI_ChatHistory*)> InFun);
	void ClearClicked();

protected:
	UFUNCTION()
	void OnAddSlot();

	UFUNCTION()
	void OnDeleteSlot();
	
};
