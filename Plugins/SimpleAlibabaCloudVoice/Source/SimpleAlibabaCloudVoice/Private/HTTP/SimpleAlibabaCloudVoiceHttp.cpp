// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "HTTP/SimpleAlibabaCloudVoiceHttp.h"
#include "SimpleAlibabaCloudVoiceType.h"

namespace SimpleAlibabaCloudVoiceHttp
{
	FHTTP::FHTTP(FHTTPDelegate InDelegate)
		:Delegate(InDelegate)
	{
		bLeaveUnused = true;
	}

	TSharedPtr<FHTTP> FHTTP::CreateHTTPObject(FHTTPDelegate InDelegate)
	{
		return MakeShareable(new FHTTP(InDelegate));
	}

	bool FHTTP::Request(
		const FString& InURL,
		const FString& InContent,//xml json
		const TMap<FString, FString>& InCustomMetadataHeader,
		EHTTPVerbType VerbType)
	{
		if (bLeaveUnused)
		{
			bLeaveUnused = false;

			TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
			
			Request->SetURL(InURL);

			for (auto& Tmp : InCustomMetadataHeader)
			{
				Request->SetHeader(Tmp.Key, Tmp.Value);
			}

			Request->SetVerb(HTTPVerbToString(VerbType));

			Request->SetContentAsString(InContent);

			Request->OnProcessRequestComplete().BindSP(this, &FHTTP::OnRequestComplete);
			Request->OnRequestProgress().BindSP(this, &FHTTP::OnRequestProgress);
			Request->OnHeaderReceived().BindSP(this, &FHTTP::OnRequestHeaderReceived);

			return Request->ProcessRequest();
		}

		return false;
	}

	FString FHTTP::HTTPVerbToString(EHTTPVerbType InVerbType)
	{
		switch (InVerbType)
		{
		case EHTTPVerbType::POST:
			return TEXT("POST");
		case EHTTPVerbType::PUT:
			return TEXT("PUT");
		case EHTTPVerbType::GET:
			return TEXT("GET");
		case EHTTPVerbType::DELETE:
			return TEXT("DELETE");
		}

		return TEXT("POST");
	}

	void FHTTP::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
	{
		bLeaveUnused = true;

		Delegate.SimpleCompleteDelegate.ExecuteIfBound(HttpRequest, HttpResponse, bSucceeded);
	}

	void FHTTP::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived)
	{
		Delegate.SimpleSingleRequestProgressDelegate.ExecuteIfBound(HttpRequest, SendBytes, BytesReceived);
	}

	void FHTTP::OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue)
	{
		Delegate.SimpleSingleRequestHeaderReceivedDelegate.ExecuteIfBound(Request, HeaderName, NewHeaderValue);
	}
}