#pragma once
#include  "Object/ChatGPTObject.h"

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

bool UChatGPTObject::RequestByGPTParam(const FString& InURL, const FChatGPTCompletionParam& param, const TMap<FString, FString> MetaDataHeader)
{
	return HTTP->Request(InURL, param, MetaDataHeader);
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
