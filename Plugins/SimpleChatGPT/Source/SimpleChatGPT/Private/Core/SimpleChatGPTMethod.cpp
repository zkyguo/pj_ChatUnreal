#pragma once
#include "Core/SimpleChatGPTMethod.h"

namespace SimpleChatGPTMethod
{
	FString EChatGPTModelToString(EChatGPTModel	model)
	{
		switch (model)
		{
		case EChatGPTModel::DAVINCI_003: return TEXT("text-davinci-003");
		case EChatGPTModel::GPT_MAX: break;
		default:;
		}

		return TEXT("");
	}

	EChatGPTModel StringToEChatGPTModel(const FString& model)
	{
		if(model.Equals("text-davinci-003"))
		{
			return EChatGPTModel::DAVINCI_003;
		}

		return EChatGPTModel::NONE;
		
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

	void StringToFChatGPTCompletionResponse(const FString& Json, FChatGPTCompletionResponse& OutResponse)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Json);
		TSharedPtr<FJsonValue> ReadRoot;

		if(FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			if(TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
			{
				OutResponse.ID = InJsonObject->GetStringField(TEXT("id"));
				OutResponse.Object = InJsonObject->GetStringField(TEXT("object"));
				OutResponse.Created = InJsonObject->GetIntegerField(TEXT("created"));
				OutResponse.Mode = StringToEChatGPTModel(InJsonObject->GetStringField(TEXT("model")));

				if(TSharedPtr<FJsonObject> UsageJsonObject = InJsonObject->GetObjectField(TEXT("usage")))
				{
					OutResponse.Usage.PromptTokens = UsageJsonObject->GetIntegerField(TEXT("prompt_tokens"));
					OutResponse.Usage.CompletionTokens = UsageJsonObject->GetIntegerField(TEXT("completion_tokens"));
					OutResponse.Usage.TotalTokens = UsageJsonObject->GetIntegerField(TEXT("total_tokens"));
				}

				const TArray<TSharedPtr<FJsonValue>>* OutArray = NULL;
				if(InJsonObject->TryGetArrayField(TEXT("choices"), OutArray))
				{
					for (auto&Tmp : *OutArray)
					{
						if(TSharedPtr<FJsonObject> ChoiceJsonObject = Tmp->AsObject() )
						{
							FChatGPTChoice &Choice = OutResponse.Choices.AddDefaulted_GetRef();
							Choice.Text = ChoiceJsonObject->GetStringField(TEXT("text"));
							Choice.Index = ChoiceJsonObject->GetIntegerField(TEXT("index"));
							Choice.FinishReason = ChoiceJsonObject->GetStringField(TEXT("finish_reason"));

							if(TSharedPtr<FJsonObject> LogprobsJsonObject = ChoiceJsonObject->GetObjectField(TEXT("logprobs")))
							{
								const  TArray<TSharedPtr<FJsonValue>>* tokenArray = NULL;
								if(LogprobsJsonObject->TryGetArrayField(TEXT("tokens"), tokenArray))
								{
									for (auto& tokenTmp : *tokenArray)
									{
										Choice.logProbs.Tokens.Add(tokenTmp->AsString());
									}
								}

								const  TArray<TSharedPtr<FJsonValue>>* logArray = NULL;
								if (LogprobsJsonObject->TryGetArrayField(TEXT("tokens_logprobs"), logArray))
								{
									for (auto& logTmp : *logArray)
									{
										Choice.logProbs.Tokens_logprobs.Add(logTmp->AsNumber());
									}
								}

								const  TArray<TSharedPtr<FJsonValue>>* offsetArray = NULL;
								if (LogprobsJsonObject->TryGetArrayField(TEXT("text_offset"), offsetArray))
								{
									for (auto& offsetTmp : *offsetArray)
									{
										int32 &InTextOffset = Choice.logProbs.Text_offset.AddDefaulted_GetRef();
										if(offsetTmp->TryGetNumber(InTextOffset) )
										{
											
										}
									}
								}
							}

						}
					}
				}
			}
		}

	}
}
