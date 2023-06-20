#pragma once
#include "Core/SimpleChatGPTMethod.h"

namespace SimpleChatGPTMethod
{
	FString GetGPTURL(EChatGPTModel model)
	{
		FString UrlSuffix;
		switch (model) {

		case EChatGPTModel::TEXT_DAVINCI_003: 
		case EChatGPTModel::TEXT_DAVINCI_002: 
		case EChatGPTModel::CODE_DAVINCI_002: 
			UrlSuffix = TEXT("v1/completions");
			break;
		case EChatGPTModel::GPT_3_5_TURBO: 
		case EChatGPTModel::GPT_4: 
		case EChatGPTModel::GPT_4_32k:
			UrlSuffix = TEXT("v1/chat/completions");
			break;
		}

		return FString::Printf(TEXT("https://api.openai.com/%s"),*UrlSuffix);
	}

	FString EChatGPTModelToString(EChatGPTModel	model)
	{
		switch (model)
		{
		case EChatGPTModel::TEXT_DAVINCI_003: return TEXT("text-davinci-003");
		case EChatGPTModel::TEXT_DAVINCI_002: return TEXT("text-davinci-002");
		case EChatGPTModel::CODE_DAVINCI_002: return TEXT("code-davinci-003");;
		case EChatGPTModel::GPT_3_5_TURBO: return TEXT("gpt-3.5-turbo");;
		case EChatGPTModel::GPT_4: return TEXT("gpt-4");
		case EChatGPTModel::GPT_4_32k: return TEXT("gpt-4-32k");
		case EChatGPTModel::GPT_MAX: break;
		default:;
		}

		return TEXT("");
	}

	EChatGPTModel StringToEChatGPTModel(const FString& model)
	{
		if(model.Equals(TEXT("text-davinci-003"), ESearchCase::IgnoreCase))
		{
			return EChatGPTModel::TEXT_DAVINCI_003;
		}
		else if (model.Equals(TEXT("text-davinci-002"), ESearchCase::IgnoreCase))
		{
			return EChatGPTModel::TEXT_DAVINCI_002;
		}
		else if (model.Equals(TEXT("code-davinci-002"), ESearchCase::IgnoreCase))
		{
			return EChatGPTModel::CODE_DAVINCI_002;
		}
		else if (model.Equals(TEXT("gpt-3.5-turbo"), ESearchCase::IgnoreCase))
		{
			return EChatGPTModel::GPT_3_5_TURBO;
		}
		else if (model.Equals(TEXT("gpt-4"), ESearchCase::IgnoreCase))
		{
			return EChatGPTModel::GPT_4;
		}
		else if (model.Equals(TEXT("gpt-4-32k"), ESearchCase::IgnoreCase))
		{
			return EChatGPTModel::GPT_4_32k;
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
			if(!param.Stop.IsEmpty())
			{
				JsonWriter->WriteValue(TEXT("stop"), param.Stop);
			}
			
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

							if(ChoiceJsonObject->HasField(TEXT("message")))
							{
								if(const TSharedPtr<FJsonObject>& InMessageObject = ChoiceJsonObject->GetObjectField(TEXT("message")))
								{
									Choice.Message.Role = StringToChatGPTRole(InMessageObject->GetStringField(TEXT("role")));
									Choice.Message.Content = InMessageObject->GetStringField(TEXT("content"));
									Choice.Message.Name = InMessageObject->GetStringField(TEXT("name"));
								}
							}

						}
					}
				}
			}
		}

	}

	FString EChatGPTProtocolToString(EChatGPTProtocol Protocol)
	{
		return UEnum::GetValueAsString(Protocol).RightChop(FString(TEXT("EChatGPTProtocol::")).Len());
	}

	FString EChatGPTImageSizeTypeToString(EChatGPTImageSizeType ImageSizeType)
	{
		return UEnum::GetValueAsString(ImageSizeType).RightChop(FString(TEXT("EChatGPTImageSizeType::IMG_")).Len());
	}

	FString EChatGPTImageEncodingTypeToString(EChatGPTImageEncodingType EncodingType)
	{
		switch (EncodingType) {
			case EChatGPTImageEncodingType::IMG_URL: 
				return TEXT("url");
			case EChatGPTImageEncodingType::IMG_Base64:
				return TEXT("b64_json");
		}

		return TEXT("");
	}

	void EChatGPtImageGenerationParamToString(FChatGPTImageGenerationParam param, FString& OutString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		JsonWriter->WriteObjectStart();
		{
			JsonWriter->WriteValue(TEXT("prompt"), param.Prompt);
			JsonWriter->WriteValue(TEXT("num_images"), param.ImageNumber);
			JsonWriter->WriteValue(TEXT("size"), EChatGPTImageSizeTypeToString(param.ImageSizeType));
			JsonWriter->WriteValue(TEXT("response_format"), EChatGPTImageEncodingTypeToString(param.ImageEncodingType));
		}

		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}

	void StringToImageResponse(const FString& Json, TArray<FString>& OutData, FString& OutType)
	{
		TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(Json);
		TSharedPtr<FJsonValue> ReadRoot;

		if (FJsonSerializer::Deserialize(JsonReader, ReadRoot))
		{
			if (TSharedPtr<FJsonObject> InJsonObject = ReadRoot->AsObject())
			{
				TArray<TSharedPtr<FJsonValue>> DataArray = InJsonObject->GetArrayField("data");
				for (auto &Tmp : DataArray)
				{
					TSharedPtr<FJsonObject> SubObject = Tmp->AsObject();
					if(SubObject->HasField("b64_json"))
					{
						OutType = TEXT("b64_json");
						OutData.Add(SubObject->GetStringField(OutType));
					}
					else if(SubObject->HasField("url"))	
					{
						OutType = TEXT("url");
						OutData.Add(SubObject->GetStringField(OutType));
					}
					else
					{
						OutType = TEXT("Failed to obtain Image data");
					}
				}
			}
		}
	}

	void ChatGPTRoleToString(EChatGPTRole InRole, FString& OutString)
	{
		OutString = UEnum::GetValueAsString(InRole).RightChop(FString(TEXT("EChatGPTRole::")).Len()).ToLower();
	}

	FString ChatGPTRoleToString(EChatGPTRole InRole)
	{
		return UEnum::GetValueAsString(InRole).RightChop(FString(TEXT("EChatGPTRole::")).Len()).ToLower();
	}

	EChatGPTRole StringToChatGPTRole(const FString& InString)
	{
		if(InString.Equals("user", ESearchCase::IgnoreCase))
		{
			return EChatGPTRole::USER;
		}
		else if(InString.Equals("assistant", ESearchCase::IgnoreCase))
		{
			return EChatGPTRole::ASSISTANT;
		}
		else if (InString.Equals("system", ESearchCase::IgnoreCase))
		{
			return EChatGPTRole::SYSTEM;
		}

		return EChatGPTRole::USER;
	}

	void ChatGPTCompletionContextParamToString(const FString& NewContent, const FChatGPTCompletionContextParam& Param, FString& OutString)
	{
		TSharedPtr<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter =
			TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&OutString);

		
		JsonWriter->WriteObjectStart();//{
		{
			//Add preview chat to message
			JsonWriter->WriteValue(TEXT("model"), EChatGPTModelToString(Param.Mode));
			JsonWriter->WriteValue(TEXT("max_tokens"), Param.MaxToken);
			JsonWriter->WriteValue(TEXT("temperature"), Param.Temperature);

			JsonWriter->WriteArrayStart(TEXT("messages"));
			for(auto &tmp : Param.Messages)
			{
				JsonWriter->WriteObjectStart();
				JsonWriter->WriteValue(TEXT("role"), ChatGPTRoleToString(tmp.Role));
				JsonWriter->WriteValue(TEXT("content"), tmp.Content);
				if(!tmp.Name.IsEmpty())
				{
					JsonWriter->WriteValue(TEXT("name"), tmp.Name);
				}

				JsonWriter->WriteObjectEnd();
			}
			//Add current question to messages
			JsonWriter->WriteObjectStart();
			{
				JsonWriter->WriteValue(TEXT("role"), TEXT("user"));
				JsonWriter->WriteValue(TEXT("content"), NewContent);
			}
			JsonWriter->WriteObjectEnd();

			JsonWriter->WriteArrayEnd();
		}

		JsonWriter->WriteObjectEnd();
		JsonWriter->Close();
	}
}
