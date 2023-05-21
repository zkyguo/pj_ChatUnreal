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

	bool FHTTP::Request(const FString& InURL, const FString& Contents, const TMap<FString, FString> MetaDataHeader, EHttpVerbType requestType)
	{
		if(!OpenAiKey.IsEmpty())
		{
			if (NotInUsed)
			{
				// https://api.openai.com/v1/chat/completions 

				NotInUsed = false;
				TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
				Request->SetURL(InURL);
				Request->SetHeader(TEXT("Content-Type"), FString("application/json"));
				Request->SetHeader(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *OpenAiKey));

				for (auto& Tmp : MetaDataHeader)
				{
					Request->SetHeader(Tmp.Key, Tmp.Value);
				}

				Request->SetVerb(HttpVerbToString(requestType));
				Request->SetContentAsString(Contents);

				Request->OnProcessRequestComplete().BindSP(this, &FHTTP::OnRequestComplete);
				Request->ProcessRequest();
				return true;
			}
		}
		

		return false;
	}


	bool FHTTP::Request(const FString& InURL, const FChatGPTCompletionParam& param,
		const TMap<FString, FString> MetaDataHeader, EHttpVerbType requestType)
	{
		FString ParamJson;
		SimpleChatGPTMethod::FChatGPTCompletionParamToString(param, ParamJson);
		return Request(InURL, ParamJson, MetaDataHeader, requestType);
	}

	void FHTTP::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces)
	{
		NotInUsed = true;
		Delegate.HttpCompleteDelegate.ExecuteIfBound(HttpRequest, HttpResponse, IsSucces);
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
