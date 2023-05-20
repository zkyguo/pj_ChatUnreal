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
		if (NotInUsed)
		{
			NotInUsed = false;
		}

		return false;
	}

	void FHTTP::OnRequestComplete(FHttpRequestPtr* HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces)
	{
		NotInUsed = true;
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
