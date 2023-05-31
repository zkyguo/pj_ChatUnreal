// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "SimpleAlibabaCloudVoiceType.h"

FNlsUserInfo::FNlsUserInfo()
{
	ExpireTime = 0;
	URL = TEXT("https://nls-gateway-cn-shanghai.aliyuncs.com/stream/v1/tts");
}

FNlsLogConfig::FNlsLogConfig()
{
	NlsPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
	LogFileSize = 400;
	LogFileNum = 50;
}

FNlsParamCallBack::FNlsParamCallBack()
{
	ID = FGuid::NewGuid();
	Event = FPlatformProcess::GetSynchEventFromPool();
	bEnableGlobalConfig = false;
	Type = ENlsParamCallBackType::CALLBACK_TIMEOUT;
	bAutomaticallyPlaySound = true;
	Handle = INDEX_NONE;
}

FNlsParamCallBack::~FNlsParamCallBack()
{
	FPlatformProcess::ReturnSynchEventToPool(Event);
}

FNlsInfo::FNlsInfo()
{
	EncodingFormat = ENlsAudioEncodingFormat::PCM;
	SampleRate = 16000;
	bIntermediateResult = true;
	bPunctuationPrediction = true;
	bInverseTextNormalization = true;
	MaxSentenceSilence = 800;
	OutputFormat = TEXT("UTF-8");

	bEnableVoiceDetection = false;
	MaxEndSilence = 800;
	MaxStartSilence = 800;
	bAutomaticallyPlaySound = true;
}

FString FNlsInfo::GetEncodingFormatString() const
{
	switch (EncodingFormat)
	{
	case ENlsAudioEncodingFormat::PCM:
		return TEXT("pcm");
	case ENlsAudioEncodingFormat::OPUS:
		return TEXT("opus");
	}

	return TEXT("pcm");
}

ENlsAudioEncodingFormat FNlsInfo::GetNlsAudioEncodingFormatByString(const FString& InString)
{
	if (InString.Equals(TEXT("pcm"), ESearchCase::IgnoreCase))
	{
		return ENlsAudioEncodingFormat::PCM;
	}
	else if (InString.Equals(TEXT("opus"), ESearchCase::IgnoreCase))
	{
		return ENlsAudioEncodingFormat::OPUS;
	}

	return ENlsAudioEncodingFormat::PCM;
}

FNlsEvent::FNlsEvent()
	:StatusCode(INDEX_NONE)
	,MsgType(ENlsEventType::TaskFailed)
	,SentenceTimeOutStatus(INDEX_NONE)
	,SentenceIndex(INDEX_NONE)
	,SentenceTime(INDEX_NONE)
	,SentenceBeginTime(INDEX_NONE)
	,SentenceConfidence(INDEX_NONE)
{

}

FSpeechSynthesizerTTSRequest::FSpeechSynthesizerTTSRequest()
{
	Voice = TEXT("xiaoyun");
	SampleRate = 16000;
	Format = TEXT("wav");
	bEnableSubtitle = true;
}

FSpeechSynthesizerPayload::FSpeechSynthesizerPayload()
{
	bEnableNotify = false;
}

FSpeechSynthesizerSentences::FSpeechSynthesizerSentences()
{
	BeginTime = 0;
	EndTime = 0;
}

FSpeechSynthesizerResponded::FSpeechSynthesizerResponded()
{
	Status = INDEX_NONE;
	ErrorCode = INDEX_NONE;
}

FRecordingConfig::FRecordingConfig()
{
	SampleRate = 16000;
	Channels = 1;
	BitsPerSample = 16;
	BufferSize = 1024 * 10;
	ExtractDataTime = 200;
}