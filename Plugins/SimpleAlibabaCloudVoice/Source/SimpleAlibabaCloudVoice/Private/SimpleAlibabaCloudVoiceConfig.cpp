// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "SimpleAlibabaCloudVoiceConfig.h"
#include "SimpleAlibabaCloudVoiceLog.h"

FSimpleAlibabaCloudVoiceConfig* FSimpleAlibabaCloudVoiceConfig::Manage = NULL;

FSimpleAlibabaCloudVoiceConfig::FSimpleAlibabaCloudVoiceConfig()
{

}

FSimpleAlibabaCloudVoiceConfig::~FSimpleAlibabaCloudVoiceConfig()
{
	
}

FSimpleAlibabaCloudVoiceConfig* FSimpleAlibabaCloudVoiceConfig::Get()
{
	if (!Manage)
	{
		Manage = new FSimpleAlibabaCloudVoiceConfig();
	}

	return Manage;
}

void FSimpleAlibabaCloudVoiceConfig::Destroy()
{
	if (Manage)
	{
		delete Manage;
	}

	Manage = NULL;
}

void FSimpleAlibabaCloudVoiceConfig::Init(const FString& InPath)
{
	FString FullPath = FPaths::ConvertRelativePathToFull(InPath);

	if (GConfig)
	{
		auto GetStringValue = [&](const TCHAR* Section, const TCHAR* Key, FString& Value)
		{
			if (!GConfig->GetString(Section, Key, Value, FullPath))
			{
				GConfig->SetString(Section, Key, *Value, FullPath);

				GConfig->Flush(false, FullPath);
			}
		};

		auto GetInt32Value = [&](const TCHAR* Section, const TCHAR* Key, int32& Value)
		{
			if (!GConfig->GetInt(Section, Key, Value, FullPath))
			{
				GConfig->SetInt(Section, Key, Value, FullPath);

				GConfig->Flush(false, FullPath);
			}
		};

		auto GetBoolValue = [&](const TCHAR* Section, const TCHAR* Key,bool& Value)
		{
			if (!GConfig->GetBool(Section, Key, Value, FullPath))
			{
				GConfig->SetBool(Section, Key, Value, FullPath);

				GConfig->Flush(false, FullPath);
			}
		};

		const TCHAR* NlsLogSection = TEXT("NlsLogConfig");

		GetStringValue(NlsLogSection, TEXT("NlsPath"), LogConfig.NlsPath);
		GetInt32Value(NlsLogSection, TEXT("LogFileSize"), LogConfig.LogFileSize);
		GetInt32Value(NlsLogSection, TEXT("LogFileNum"), LogConfig.LogFileNum);

		const TCHAR* NlsUserSection = TEXT("NlsUserConfig");

		GetInt32Value(NlsUserSection, TEXT("ExpireTime"), UserInfo.ExpireTime);
		GetStringValue(NlsUserSection, TEXT("AppKey"), UserInfo.AppKey);
		GetStringValue(NlsUserSection, TEXT("ID"), UserInfo.ID);
		GetStringValue(NlsUserSection, TEXT("Secret"), UserInfo.Secret);
		GetStringValue(NlsUserSection, TEXT("URL"), UserInfo.URL);

		const TCHAR* NlsSection = TEXT("NlsSection");
		int32 EncodingFormatInt = (int32)Info.EncodingFormat;
		GetInt32Value(NlsSection, TEXT("EncodingFormat"), EncodingFormatInt);
		Info.EncodingFormat = (ENlsAudioEncodingFormat)EncodingFormatInt;

		GetInt32Value(NlsSection, TEXT("SampleRate"), Info.SampleRate);
		GetInt32Value(NlsSection, TEXT("MaxStartSilence"), Info.MaxStartSilence);
		GetInt32Value(NlsSection, TEXT("MaxEndSilence"), Info.MaxEndSilence);
		GetBoolValue(NlsSection, TEXT("bEnableVoiceDetection"), Info.bEnableVoiceDetection);
		GetBoolValue(NlsSection, TEXT("bIntermediateResult"), Info.bIntermediateResult);
		GetBoolValue(NlsSection, TEXT("bPunctuationPrediction"), Info.bPunctuationPrediction);
		GetBoolValue(NlsSection, TEXT("bInverseTextNormalization"), Info.bInverseTextNormalization);
		GetInt32Value(NlsSection, TEXT("MaxSentenceSilence"), Info.MaxSentenceSilence);
		GetStringValue(NlsSection, TEXT("CustomizationId"), Info.CustomizationId);
		GetStringValue(NlsSection, TEXT("VocabularyId"), Info.VocabularyId);
		GetStringValue(NlsSection, TEXT("PayloadParam"), Info.PayloadParam);
		GetStringValue(NlsSection, TEXT("OutputFormat"), Info.OutputFormat);
	}
}

void FSimpleAlibabaCloudVoiceConfig::Save(const FString& InPath)
{
	FString FullPath = FPaths::ConvertRelativePathToFull(InPath);

	auto SetInt32Value = [&](const TCHAR* Section, const TCHAR* Key, int32 Value)
	{
		GConfig->SetInt(Section, Key, Value, FullPath);

		GConfig->Flush(false, FullPath);
	};

	const TCHAR* NlsUserSection = TEXT("NlsUserConfig");
	SetInt32Value(NlsUserSection,TEXT("ExpireTime"),UserInfo.ExpireTime);
}