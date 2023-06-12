#pragma once
#include "CoreMinimal.h"
#include "Json.h"
#include "SimpleChatGPTType.h"

namespace SimpleChatGPTMethod
{
	FString SIMPLECHATGPT_API GetGPTURL(EChatGPTModel	model);
	FString SIMPLECHATGPT_API EChatGPTModelToString(EChatGPTModel	model);
	EChatGPTModel SIMPLECHATGPT_API StringToEChatGPTModel(const FString& model);

	void SIMPLECHATGPT_API FChatGPTCompletionParamToString(const FChatGPTCompletionParam param, FString &Outjson);
	void SIMPLECHATGPT_API StringToFChatGPTCompletionResponse(const FString &Json, FChatGPTCompletionResponse& OutResponse);

	FString SIMPLECHATGPT_API EChatGPTProtocolToString(EChatGPTProtocol	Protocol);
	FString SIMPLECHATGPT_API EChatGPTImageSizeTypeToString(EChatGPTImageSizeType ImageSizeType);
	FString SIMPLECHATGPT_API EChatGPTImageEncodingTypeToString(EChatGPTImageEncodingType EncodingType);

	void SIMPLECHATGPT_API EChatGPtImageGenerationParamToString(FChatGPTImageGenerationParam param, FString &OutString);
	void SIMPLECHATGPT_API StringToImageResponse(const FString& Json, TArray<FString> &OutData, FString &OutType);
}
