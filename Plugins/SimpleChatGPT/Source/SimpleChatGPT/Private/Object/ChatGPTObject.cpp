#pragma once
#include  "Object/ChatGPTObject.h"
#include "Core/SimpleChatGPTMethod.h"

UChatGPTObject::UChatGPTObject()
{
}

void UChatGPTObject::SetOpenAiKey(const FString& Key)
{
	HTTP->SetKOpenAiKey(Key);
}

UChatGPTObject* UChatGPTObject::CreateObject(UObject* Worldcontext,UClass* InClass, UObject* parent)
{
	check(Worldcontext != NULL);

	UClass* myclass = InClass;
	if(!InClass)
	{
		myclass = UChatGPTObject::StaticClass();
	}

	if(parent)
	{
		//Create UObject under parent
		if (UChatGPTObject* obj = NewObject<UChatGPTObject>(parent, myclass))
		{
			obj->InitChatGPT();
			return obj;
		}
	}
	else
	{
		if (UChatGPTObject* obj = NewObject<UChatGPTObject>(Worldcontext, myclass))
		{
			obj->InitChatGPT();
			return obj;
		}
	}
	return NULL;
}

bool UChatGPTObject::Request(const FString& InURL, const FString& Contents, const TMap<FString, FString> MetaDataHeader)
{
	return HTTP->Request(InURL, Contents, MetaDataHeader);
}

bool UChatGPTObject::RequestImage(const FChatGPTImageGenerationParam& param,
	const TMap<FString, FString> MetaDataHeader)
{
	return HTTP->Request(param, MetaDataHeader);
}

bool UChatGPTObject::RequestByGPTParam(EChatGPTModel mode, const FChatGPTCompletionParam& param,
                                       const TMap<FString, FString> MetaDataHeader)
{
	return HTTP->Request(mode, param, MetaDataHeader);
}

bool UChatGPTObject::SimpleRequest(const FChatGPTCompletionParam& param, const TMap<FString, FString> MetaDataHeader)
{
	return HTTP->Request(param.Mode, param, MetaDataHeader);
}


bool UChatGPTObject::IsNotInUse() const
{
	return HTTP->IsNotInUse();
}

void UChatGPTObject::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
{
	if (bSucceeded)
	{
		int32 ResponseCode = HttpResponse->GetResponseCode();
		if (ResponseCode == 200)
		{
			if(HttpRequest->GetHeader(TEXT("Access-Protocol")).Equals(SimpleChatGPTMethod::EChatGPTProtocolToString(EChatGPTProtocol::ChatGPT_TEXT)))
			{
				if (OnSucces.IsBound())
				{
					FString JsonString = HttpResponse->GetContentAsString();

					FChatGPTCompletionResponse ChatGPTCompletionResponses;
					SimpleChatGPTMethod::StringToFChatGPTCompletionResponse(JsonString, ChatGPTCompletionResponses);

					TArray<FString> TextContent;
					for (auto& Tmp : ChatGPTCompletionResponses.Choices)
					{
						TextContent.Add(Tmp.Text);
					}

					OnSucces.Broadcast(TextContent, TEXT(""));

					return;
				}
			}
			else if (HttpRequest->GetHeader(TEXT("Access-Protocol")).Equals(SimpleChatGPTMethod::EChatGPTProtocolToString(EChatGPTProtocol::ChatGPT_IMAGE)))
			{
				
			}
		}
	}

	if (OnFailure.IsBound())
	{
		TArray<FString> ErrorData;
		OnFailure.Broadcast(ErrorData, TEXT("Error"));
	}
}

void UChatGPTObject::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 TotalBytes, int32 BytesReceived)
{
	if(HttpRequest->GetResponse())
	{
		OnProgress.Broadcast(HttpRequest->GetResponse()->GetContent(), TotalBytes, BytesReceived);
	}
	
}

void UChatGPTObject::OnRequestHeaderReceived(FHttpRequestPtr HttpRequest, const FString& HeaderName,
	const FString& NewHeaderReceived)
{
	OnReceivedResponse.Broadcast(HeaderName, NewHeaderReceived);
}

void UChatGPTObject::InitChatGPT()
{
	ChatHttp::FHTTPDelegate HttpDelegate;
	HttpDelegate.HttpCompleteDelegate.BindLambda(

		[&](FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool isSucceeded)
		{
			OnRequestComplete(HttpRequest, HttpResponse, isSucceeded);
		}
	);

	HttpDelegate.HttpProgressDelegate.BindLambda(

		[&](FHttpRequestPtr HttpRequest, int32 totalBytes, int32 BytesReceived)
		{
			OnRequestProgress(HttpRequest, totalBytes, BytesReceived);
		}

	);

	HttpDelegate.HttpHeaderReceiverDelegate.BindLambda(

		[&](FHttpRequestPtr HttpRequest, const FString& HeaderName,const FString& NewHeaderReceived)
		{
			OnRequestHeaderReceived(HttpRequest, HeaderName,NewHeaderReceived);
		}
	);

	HTTP = ChatHttp::FHTTP::CreateHTTPObject(HttpDelegate);
}
