#pragma once
#include  "Object/ChatGPTObject.h"

UChatGPTObject::UChatGPTObject()
{
}

UChatGPTObject* UChatGPTObject::CreateObject(UClass* InClass, UObject* parent)
{
	if(parent)
	{
		//Create UObject under parent
		if (UChatGPTObject* obj = NewObject<UChatGPTObject>(parent, InClass))
		{
			obj->InitChatGPT();
			return obj;
		}
	}
	else
	{
		if (UChatGPTObject* obj = NewObject<UChatGPTObject>(NULL, InClass))
		{
			obj->AddToRoot();
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

bool UChatGPTObject::IsNotInUse() const
{
	return HTTP->IsNotInUse();
}

void UChatGPTObject::OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool IsSucces)
{
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

	HTTP = ChatHttp::FHTTP::CreateHTTPObject(HttpDelegate);
}
