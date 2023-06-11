// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Chat/UI_ChatMain.h"
#include "Core/SimpleChatGPTMethod.h"

UUI_ChatMain::UUI_ChatMain()
{
	isLocalLink = true;
}

void UUI_ChatMain::NativeConstruct()
{
	Super::NativeConstruct();
	ChatHttp::FHTTPDelegate HttpDelegate;
	HttpDelegate.HttpCompleteDelegate.BindLambda(

		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool isSucceeded)
		{
			OnRequestComplete(HttpRequest, HttpResponse, isSucceeded);
		}
	);

	HTTP = ChatHttp::FHTTP::CreateHTTPObject(HttpDelegate);
	HTTP->SetKOpenAiKey("");
	
}

void UUI_ChatMain::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UUI_ChatMain::OnSendRequest(int32 InID, const FText& Content)
{
	if(isLocalLink)
	{
		//HTTP
		if(HTTP)
		{
			TMap<FString, FString> CustomMetadataHeader;
			CustomMetadataHeader.Add(TEXT("Chatter_ID"), FString::FromInt(InID));

			FChatGPTCompletionParam Param;
			Param.Prompt = Content.ToString();
			Param.Mode = EChatGPTModel::TEXT_DAVINCI_003;


			HTTP->Request(Param, CustomMetadataHeader);
		}
	}
	else 
	{
		
	}

}

bool UUI_ChatMain::IsNotInUse() const
{
	return HTTP->IsNotInUse();
}

void UUI_ChatMain::InitSimpleChatGPT()
{
	
}

void UUI_ChatMain::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces)
{
	if(IsSucces)
	{
		int32 ResponseCode = HttpResponse->GetResponseCode();
		if (ResponseCode == 200)
		{
			FString JsonString = HttpResponse->GetContentAsString();

			FChatGPTCompletionResponse ChatGPTCompletionResponses;
			SimpleChatGPTMethod::StringToFChatGPTCompletionResponse(JsonString, ChatGPTCompletionResponses);

			for (auto& Tmp : ChatGPTCompletionResponses.Choices)
			{
				if(Tmp.Text.IsEmpty())
				{
					FText InContent = FText::FromString(TEXT("No GPT answer obtained"));
					ChatList->AddResponseChat(2, FText::FromString(TEXT("No GPT answer obtained")));
					ChatList->SubmitChat(2, FText::FromString(TEXT("No GPT answer obtained")));
				}
				else
				{
					FText InContent = FText::FromString(Tmp.Text);
					ChatList->AddResponseChat(2, InContent);
					ChatList->SubmitChat(2, InContent);
				}
				
			}

			return;
		}
		else if(ResponseCode == 400)
		{
			FText InContent = FText::FromString(TEXT("Request Error"));
			ChatList->AddResponseChat(2, InContent);
			ChatList->SubmitChat(2, InContent);
		}
		else if (ResponseCode == 429)
		{
			FText InContent = FText::FromString(TEXT("Too many request !"));
			ChatList->AddResponseChat(2, InContent);
			ChatList->SubmitChat(2, InContent);
		}
		else if (ResponseCode == 401)
		{
			FText InContent = FText::FromString(TEXT("You access is not permis, please verify API key"));
			ChatList->AddResponseChat(2, InContent);
			ChatList->SubmitChat(2, InContent);
		}
	}
	else
	{
		FText InContent = FText::FromString(TEXT("Connection Error"));
		ChatList->AddResponseChat(2, InContent);
		ChatList->SubmitChat(2, InContent);
	}
}
