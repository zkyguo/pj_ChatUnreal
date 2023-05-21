#pragma once
#include "Core/SimpleChatGPTMethod.h"

namespace SimpleChatGPTMethod
{
	FString EChatGPTModelToString(EChatGPTModel	model)
	{
		switch (model)
		{
		case EChatGPTModel::DAVINCI_003: return TEXT("text_davinci-003");
		case EChatGPTModel::GPT_MAX: break;
		default:;
		}

		return TEXT("");
	}

	void FChatGPTCompletionParamToString(const FChatGPTCompletionParam param, FString& Outjson)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Outjson);

		JsonWriter->WriteObjectStart();//{
		{
			JsonWriter->WriteValue(TEXT("model"), EChatGPTModelToString(param.Mode));
			JsonWriter->WriteValue(TEXT("prompt"), param.Prompt);
			JsonWriter->WriteValue(TEXT("max_tokens"), param.MaxToken);
			JsonWriter->WriteValue(TEXT("temperature"), param.Temperature);
			JsonWriter->WriteValue(TEXT("top_p"), param.Top_p);
			JsonWriter->WriteValue(TEXT("n"), param.N);
			JsonWriter->WriteValue(TEXT("stream"), param.bStream);
			JsonWriter->WriteValue(TEXT("logprobs"), NULL);
			JsonWriter->WriteValue(TEXT("stop"), param.Stop);
		}
		JsonWriter->WriteObjectEnd();//}
		JsonWriter->Close();
	}
}
