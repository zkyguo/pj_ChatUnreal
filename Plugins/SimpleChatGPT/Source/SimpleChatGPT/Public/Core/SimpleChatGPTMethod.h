#pragma once
#include "CoreMinimal.h"
#include "Json.h"
#include "SimpleChatGPTType.h"

namespace SimpleChatGPTMethod
{
	FString EChatGPTModelToString(EChatGPTModel	model);
	EChatGPTModel StringToEChatGPTModel(const FString& model);

	void FChatGPTCompletionParamToString(const FChatGPTCompletionParam param, FString &Outjson);
	void StringToFChatGPTCompletionResponse(const FString &Json, FChatGPTCompletionResponse& OutResponse);

}
