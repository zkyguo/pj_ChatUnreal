// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Core/UI_Base.h"
#include "Element/UI_ChatList.h"
#include "UI_ChatHistoryList.h"
#include "HTTP/SimpleChatGPTHttp.h"
#include "UI_ChatMain.generated.h"

/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_ChatMain : public UUI_Base
{
	GENERATED_BODY()

public :
	UUI_ChatMain();
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_ChatList> ChatList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUI_ChatHistoryList> ChatHistoryList;

	UPROPERTY()
	bool isLocalLink;



public:
	void OnSendRequest(int32 InID,const FText &inText);
	bool IsNotInUse() const;

protected:
	TSharedPtr<ChatHttp::FHTTP> HTTP;
	void InitSimpleChatGPT();
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces);
};
