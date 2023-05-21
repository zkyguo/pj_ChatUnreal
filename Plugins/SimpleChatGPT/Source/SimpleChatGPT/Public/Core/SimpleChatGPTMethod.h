#pragma once
#include "CoreMinimal.h"
#include "Json.h"
#include "SimpleChatGPTType.h"

namespace SimpleChatGPTMethod
{
	FString EChatGPTModelToString(EChatGPTModel	model);
	void FChatGPTCompletionParamToString(const FChatGPTCompletionParam param, FString &Outjson);

}
