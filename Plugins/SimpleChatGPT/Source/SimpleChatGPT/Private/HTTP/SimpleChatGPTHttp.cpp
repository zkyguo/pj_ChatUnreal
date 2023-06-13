#pragma once
#include "HTTP/SimpleChatGPTHttp.h"


namespace  ChatHttp
{
	FHTTP::FHTTP(FHTTPDelegate InDelegate)
		:Delegate(InDelegate)
	{
		NotInUsed = true;
	}

	TSharedPtr<FHTTP> FHTTP::CreateHTTPObject(FHTTPDelegate InDelegate)
	{
		return MakeShareable(new FHTTP(InDelegate));
	}

	bool FHTTP::Request(const FString& InURL, 
						const FString& Contents, 
						const TMap<FString, FString> MetaDataHeader,
						EChatGPTProtocol Protocol,
						EHttpVerbType requestType)
	{
		if(!OpenAiKey.IsEmpty())
		{
			if (NotInUsed)
			{
				// https://api.openai.com/v1/completions 

				NotInUsed = false;
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
				Request->SetURL(InURL);

				Request->SetHeader(TEXT("Content-Type"), FString("application/json"));
				Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *OpenAiKey));
				Request->SetHeader(TEXT("Access-Protocol"), SimpleChatGPTMethod::EChatGPTProtocolToString(Protocol));
				Request->SetTimeout(60.f);

				if(!MetaDataHeader.IsEmpty())
				{
					for (auto& Tmp : MetaDataHeader)
					{
						if(!Tmp.Key.IsEmpty() && !Tmp.Value.IsEmpty())
						{
							Request->SetHeader(Tmp.Key, Tmp.Value);
						}
						
					}
				}
				

				Request->SetVerb(HttpVerbToString(requestType));
				Request->SetContentAsString(Contents);

				Request->OnProcessRequestComplete().BindSP(this, &FHTTP::OnRequestComplete);
				Request->OnRequestProgress().BindSP(this, &FHTTP::OnRequestProgress);
				Request->OnHeaderReceived().BindSP(this, &FHTTP::OnRequestHeaderReceived);

				return Request->ProcessRequest();
			}
		}
		

		return false;
	}


	bool FHTTP::Request(const FString& InURL, const FChatGPTCompletionParam& param,
		const TMap<FString, FString> MetaDataHeader, EHttpVerbType requestType)
	{
		FString ParamJson;
		SimpleChatGPTMethod::FChatGPTCompletionParamToString(param, ParamJson);
		return Request(InURL, ParamJson, MetaDataHeader,EChatGPTProtocol::ChatGPT_TEXT ,requestType);
	}

	bool FHTTP::Request(const EChatGPTModel mode, const FChatGPTCompletionParam& param,
		const TMap<FString, FString> MetaDataHeader, EHttpVerbType requestType)
	{
		FString ParamJson;
		FString Url = SimpleChatGPTMethod::GetGPTURL(mode);
		SimpleChatGPTMethod::FChatGPTCompletionParamToString(param, ParamJson);

		UE_LOG(ChatGPTLog, Log, TEXT("URL=%s"), *Url);
		UE_LOG(ChatGPTLog, Log, TEXT("Json=%s"), *ParamJson);

		return Request(Url, ParamJson, MetaDataHeader, EChatGPTProtocol::ChatGPT_TEXT,requestType);
	}

	bool FHTTP::Request(const FChatGPTCompletionParam& param, const TMap<FString, FString> MetaDataHeader,
		EHttpVerbType requestType)
	{
		return  Request(param.Mode, param, MetaDataHeader, requestType);
	}

	bool FHTTP::Request(const FChatGPTImageGenerationParam& param, const TMap<FString, FString> MetaDataHeader,
		EHttpVerbType requestType)
	{
		FString JsonString;
		SimpleChatGPTMethod::EChatGPtImageGenerationParamToString(param, JsonString);

		FString Url = TEXT("https://api.openai.com/v1/images/generations");
		return Request(Url, JsonString, MetaDataHeader, EChatGPTProtocol::ChatGPT_IMAGE, EHttpVerbType::POST);	
	}

	void FHTTP::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces)
	{
		NotInUsed = true;
		Delegate.HttpCompleteDelegate.ExecuteIfBound(HttpRequest, HttpResponse, IsSucces);

		if(IsSucces)
		{
			if(HttpRequest->GetHeader(TEXT("Access-Protocol")).Equals(SimpleChatGPTMethod::EChatGPTProtocolToString(EChatGPTProtocol::ChatGPT_TEXT)))
			{
				UE_LOG(ChatGPTLog, Log, TEXT("[OnRequestComplete] %s"), *HttpResponse->GetContentAsString());
			}
			else if (HttpRequest->GetHeader(TEXT("Access-Protocol")).Equals(SimpleChatGPTMethod::EChatGPTProtocolToString(EChatGPTProtocol::ChatGPT_IMAGE)))
			{
				
			}
		}
		else
		{

			UE_LOG(ChatGPTLog, Error, TEXT("[OnRequestComplete] %s"), *HttpResponse->GetContentAsString());
		}

	}

	void FHTTP::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 TotalBytes, int32 BytesReceived)
	{
		Delegate.HttpProgressDelegate.ExecuteIfBound(HttpRequest, TotalBytes, BytesReceived);

		if(HttpRequest->GetResponse())
		{
			UE_LOG(ChatGPTLog, Log, TEXT("[OnRequestProgress] %s"), *HttpRequest->GetResponse()->GetContentAsString());
		}

		
	}

	void FHTTP::OnRequestHeaderReceived(FHttpRequestPtr HttpRequest, const FString& HeaderName,
		const FString& NewHeaderReceived)
	{
		Delegate.HttpHeaderReceiverDelegate.ExecuteIfBound(HttpRequest, HeaderName, NewHeaderReceived);
	}

	FString FHTTP::HttpVerbToString(EHttpVerbType verb)
	{
		switch (verb)
		{
			case POST: return TEXT("POST");
			case PUT: return TEXT("PUT");
			case GET: return TEXT("GET");
			case DELETE: return TEXT("DELETE");
			default: ;
		}
		return TEXT("POST");
	}
}
