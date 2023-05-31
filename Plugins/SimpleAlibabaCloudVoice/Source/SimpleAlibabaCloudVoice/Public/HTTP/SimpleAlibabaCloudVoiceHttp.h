// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"

DECLARE_DELEGATE_ThreeParams(FHttpABCVRequestProgressDelegate, FHttpRequestPtr /*Request*/, int32 /*TotalBytes*/, int32 /*BytesReceived*/)

namespace SimpleAlibabaCloudVoiceHttp
{
	enum EHTTPVerbType
	{
		POST,
		PUT,
		GET,
		DELETE,
	};

	struct SIMPLEALIBABACLOUDVOICE_API FHTTPDelegate
	{
		FHttpRequestCompleteDelegate						SimpleCompleteDelegate;
		FHttpABCVRequestProgressDelegate					SimpleSingleRequestProgressDelegate;
		FHttpRequestHeaderReceivedDelegate					SimpleSingleRequestHeaderReceivedDelegate;
	};

	struct SIMPLEALIBABACLOUDVOICE_API FHTTP :public TSharedFromThis<FHTTP>
	{
		static TSharedPtr<FHTTP> CreateHTTPObject(FHTTPDelegate InDelegate);

	public:

		FHTTP(FHTTPDelegate InDelegate);

		//向服务器发送HTTP请求
		bool Request(
			const FString& InURL,
			const FString& InContent,
			const TMap<FString, FString>& InCustomMetadataHeader,
			EHTTPVerbType VerbType = EHTTPVerbType::POST);

		bool IsLeaveUnused() const { return bLeaveUnused; }

	private:
		void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);
		void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived);
		void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue);
	
	private:
		FString HTTPVerbToString(EHTTPVerbType InVerbType);

	private:
		bool bLeaveUnused;//是否闲置
		FHTTPDelegate Delegate;
	};
}