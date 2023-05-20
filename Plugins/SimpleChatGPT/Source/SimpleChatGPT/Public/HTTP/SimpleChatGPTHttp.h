#pragma once
#include "CoreMinimal.h"
#include  "http.h"

namespace  ChatHttp
{

	enum EHttpVerbType
	{
		POST,
		PUT,
		GET,
		DELETE,
	};

	struct FHTTPDelegate
	{
		FHttpRequestCompleteDelegate HttpCompleteDelegate;
	};


	struct FHTTP : public TSharedFromThis<FHTTP>
	{
		/**
		 * @brief Create Http object by Delegate
		 * @param InDelegate 
		 * @return 
		 */
		static TSharedPtr<FHTTP> CreateHTTPObject (FHTTPDelegate InDelegate);

	public :
		FHTTP(FHTTPDelegate InDelegate);

		/**
		 * @brief Send request to server
		 */
		void Request(
			const FString &InURL,
			const FString &Contents,
			const TMap<FString,FString> MetaDataHeader,
			EHttpVerbType requestType = EHttpVerbType::POST);

		bool const IsNotInUse() { return NotInUsed; }

	private:
		void OnRequestComplete(FHttpRequestPtr* HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces);
		FString HttpVerbToString(EHttpVerbType verb);
		bool NotInUsed;
		FHTTPDelegate Delegate;
	};

}
