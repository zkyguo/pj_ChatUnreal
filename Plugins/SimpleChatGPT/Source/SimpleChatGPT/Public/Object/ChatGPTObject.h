#pragma once
#include "CoreMinimal.h"
#include "HTTP/SimpleChatGPTHttp.h"
#include "ChatGPTObject.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FChatGPTHttpResponseDelegate, const TArray<FString>&, ChatData, const FString&, Message);

UCLASS(BlueprintType, Blueprintable)
class SIMPLECHATGPT_API UChatGPTObject : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "CreateChatGPTObject", Keywords = "GPT"), Category = "ChatGPT")
	static  UChatGPTObject* CreateObject(UClass* InClass = NULL, UObject* parent = NULL);

	UFUNCTION(BlueprintCallable, Category = "ChatGPT")
	bool Request(const FString& InURL, const FString& Contents, const TMap<FString, FString> MetaDataHeader);

	UFUNCTION(BlueprintPure, Category = "ChatGPT")
	bool IsNotInUse() const;

	UChatGPTObject();

	TSharedPtr<ChatHttp::FHTTP> HTTP;

	UPROPERTY(BlueprintAssignable)
	FChatGPTHttpResponseDelegate OnSucces;

	UPROPERTY(BlueprintAssignable)
	FChatGPTHttpResponseDelegate OnFailure;

protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces);
	void InitChatGPT();
};
