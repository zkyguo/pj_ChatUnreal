// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_Base.h"
#include "Components/EditableText.h"
#include <Components/Button.h>
#include <Components/ScrollBox.h>
#include "UI_Chat.h"
#include "Engine/Texture2D.h"
#include <Components/ComboBoxString.h>
#include "UI_ChatList.generated.h"





/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_ChatList : public UUI_Base
{
	GENERATED_BODY()

public :
	UUI_ChatList();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> CommandType;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableText> TextInput;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SendButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ListBox;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_Chat> ChatRightClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUI_Chat> ChatLeftClass;

	UUI_Chat* AddRequestChat(int32 ID,const FText &Content);
	UUI_Chat* AddResponseChat(int32 ID, const FText& Content);
	UUI_Chat* AddResponseChat(int32 ID, UTexture2D* Image);
	UUI_Chat* AddResponseChat(int32 ID, TArray<UTexture2D*> Images);

	void SubmitChat(int32 ID, const FText& InContent);
	void SubmitChat(int32 ID, const TArray<UTexture2D*>& InTexture);

	void UpdateChatWidget();
	void ClearChatWidget();

protected:
	UFUNCTION()
	void OnSend();

	UFUNCTION()
	void OnTextCommit(const FText& Text, ETextCommit::Type CommitMethod);
};
