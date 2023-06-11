#pragma once
#include "CoreMinimal.h"
#include  "HTTP.h"
#include "ChatGPTLog.h"
#include "Core/SimpleChatGPTMethod.h"


DECLARE_DELEGATE_ThreeParams(FHttpGPTRequestProgressDelegate, FHttpRequestPtr /*Request*/, int32 /*TotalBytes*/, int32/*BytesReceived*/ )
namespace  ChatHttp
{

	enum EHttpVerbType
	{
		POST,
		PUT,
		GET,
		DELETE,
	};

	struct SIMPLECHATGPT_API FHTTPDelegate
	{
		FHttpRequestCompleteDelegate HttpCompleteDelegate;
		FHttpGPTRequestProgressDelegate HttpProgressDelegate;
		FHttpRequestHeaderReceivedDelegate HttpHeaderReceiverDelegate;
	};


	struct SIMPLECHATGPT_API FHTTP : public TSharedFromThis<FHTTP>
	{
		/**
		 * @brief Create Http object by Delegate
		 * @param InDelegate
		 * @return
		 */
		static TSharedPtr<FHTTP> CreateHTTPObject(FHTTPDelegate InDelegate);

	public :
		FHTTP(FHTTPDelegate InDelegate);


		/**
		 * @brief Send request to server
		 */
		bool Request(
			const FString &InURL,
			const FString &Contents,
			const TMap<FString,FString> MetaDataHeader,
			EChatGPTProtocol Protocol = EChatGPTProtocol::ChatGPT_TEXT,
			EHttpVerbType requestType = EHttpVerbType::POST);

		bool IsNotInUse() const { return NotInUsed; }
		void SetKOpenAiKey(const FString& Key) { OpenAiKey = Key; }

		/**
		 * @brief Send Chat Params to server
		 * @param InURL 
		 * @param param 
		 * @param MetaDataHeader 
		 * @param requestType 
		 * @return 
		 */
		bool Request(
			const FString& InURL,
			const FChatGPTCompletionParam& param,
			const TMap<FString, FString> MetaDataHeader,
			EHttpVerbType requestType = EHttpVerbType::POST);

		bool Request(
			const EChatGPTModel mode,
			const FChatGPTCompletionParam& param,
			const TMap<FString, FString> MetaDataHeader,
			EHttpVerbType requestType = EHttpVerbType::POST);

		bool Request(
			const FChatGPTCompletionParam& param,
			const TMap<FString, FString> MetaDataHeader,
			EHttpVerbType requestType = EHttpVerbType::POST);
		
	private:
		void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces);
		void OnRequestProgress(FHttpRequestPtr HttpRequest,int32 TotalBytes, int32 BytesReceived);
		void OnRequestHeaderReceived(FHttpRequestPtr HttpRequest, const FString& HeaderName, const FString& NewHeaderReceived);

		FString HttpVerbToString(EHttpVerbType verb);
		bool NotInUsed;
		FHTTPDelegate Delegate;
		FString OpenAiKey;
	};

}
