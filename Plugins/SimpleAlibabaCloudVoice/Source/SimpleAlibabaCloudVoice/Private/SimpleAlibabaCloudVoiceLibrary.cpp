// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "SimpleAlibabaCloudVoiceLibrary.h"
#include "SimpleAlibabaCloudVoiceManage.h"
#include "Sound/SimpleSoundHandle.h"
#include "Sound/SimpleRecordingCollection.h"
#include "Sound/SimpleSoundFormatHandle.h"

void USimpleAlibabaCloudVoiceLibrary::SetNlsLogConfig(const FString& InLogPath)
{
	FSimpleAlibabaCloudVoiceManage::Get()->SetNlsLogConfig(InLogPath);
}

void USimpleAlibabaCloudVoiceLibrary::SetDirectHost(const FString& InIP)
{
	FSimpleAlibabaCloudVoiceManage::Get()->SetDirectHost(InIP);
}

void USimpleAlibabaCloudVoiceLibrary::SetUseSysGetAddrInfo(bool bEnable)
{
	FSimpleAlibabaCloudVoiceManage::Get()->SetUseSysGetAddrInfo(bEnable);
}

void USimpleAlibabaCloudVoiceLibrary::StartWorkThread(int32 InNum)
{
	FSimpleAlibabaCloudVoiceManage::Get()->StartWorkThread(InNum);
}

void USimpleAlibabaCloudVoiceLibrary::StartSpeechTranscriber(int32 &OutHandle, const FNlsInfo& InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, bool bEnableGlobalConfig)
{
	auto CallbackDelegate = [InCallbackDelegate](const FNlsEvent& InEvent)
	{
		InCallbackDelegate.ExecuteIfBound(InEvent);
	};

	FSimpleAlibabaCloudVoiceManage::Get()->StartSpeechTranscriber(OutHandle, InCofigInfo, CallbackDelegate, bEnableGlobalConfig);
}

void USimpleAlibabaCloudVoiceLibrary::StopSpeechTranscriber(int32 InHandle)
{
	FSimpleAlibabaCloudVoiceManage::Get()->StopSpeechTranscriber(InHandle);
}

void USimpleAlibabaCloudVoiceLibrary::SpeechRecognizerFile(const FString & InSpeechPath, const FNlsInfo & InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, bool bEnableGlobalConfig)
{
	auto CallbackDelegate = [InCallbackDelegate](const FNlsEvent& InEvent)
	{
		InCallbackDelegate.ExecuteIfBound(InEvent);
	};

	FSimpleAlibabaCloudVoiceManage::Get()->SpeechRecognizerFile(InSpeechPath, InCofigInfo, CallbackDelegate, bEnableGlobalConfig);
}

void USimpleAlibabaCloudVoiceLibrary::SpeechRecognizerMultFile(const TArray<FString>& InSpeechPaths, const FNlsInfo& InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, bool bEnableGlobalConfig)
{
	auto CallbackDelegate = [InCallbackDelegate](const FNlsEvent& InEvent)
	{
		InCallbackDelegate.ExecuteIfBound(InEvent);
	};

	FSimpleAlibabaCloudVoiceManage::Get()->SpeechRecognizerMultFile(InSpeechPaths, InCofigInfo, CallbackDelegate, bEnableGlobalConfig);
}

void USimpleAlibabaCloudVoiceLibrary::SpeechSynthesizerFile(const FNlsInfo& InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, const FString& InConent, const FString& InAudioFile, bool bEnableGlobalConfig)
{
	auto CallbackDelegate = [InCallbackDelegate](const FNlsEvent& InEvent)
	{
		InCallbackDelegate.ExecuteIfBound(InEvent);
	};

	FSimpleAlibabaCloudVoiceManage::Get()->SpeechSynthesizerFile(InCofigInfo, CallbackDelegate, InConent, InAudioFile, bEnableGlobalConfig);
}

int32 USimpleAlibabaCloudVoiceLibrary::GetSendAudioSleepTime(int32 DataSize, int32 SampleRate, int32 CompressRate)
{
	return FSimpleAlibabaCloudVoiceManage::Get()->GetSendAudioSleepTime(DataSize, SampleRate, CompressRate);
}

bool USimpleAlibabaCloudVoiceLibrary::GenerateToken(FString& OutToken, int32& OutExpireTime)
{
	uint32 TmpTime = 0;
	if (FSimpleAlibabaCloudVoiceManage::Get()->GenerateToken(OutToken, TmpTime))
	{
		OutExpireTime = TmpTime;
		return true;
	}
	
	return false;
}

bool USimpleAlibabaCloudVoiceLibrary::IsExpire(int32 InExpireTime)
{
	return FSimpleAlibabaCloudVoiceManage::Get()->IsExpire(InExpireTime);
}

bool USimpleAlibabaCloudVoiceLibrary::SimplePlaySoundByFile(const FString& InFilePath)
{
	return SimpleSoundHandle::SimplePlaySoundByFile(InFilePath);
}

bool USimpleAlibabaCloudVoiceLibrary::SimplePlaySoundByMemory(const TArray<uint8>& InSoundPtr)
{
	return SimpleSoundHandle::SimplePlaySoundByMemory(InSoundPtr.GetData());
}

bool USimpleAlibabaCloudVoiceLibrary::SimpleStopSound()
{
	return SimpleSoundHandle::SimpleStopSound();
}

bool USimpleAlibabaCloudVoiceLibrary::StartRecording(const FString& InSavePath, float InTime)
{
	TSharedPtr<FSimpleRecordingCollection> InRecordingCollection = FSimpleRecordingCollection::CreateRecordingCollection();

	FRecordingConfig InConfig;

	float ExtractDataTime = InConfig.ExtractDataTime;
	InTime *= 1000;

	int32 LoopCount = InTime / ExtractDataTime;

	InRecordingCollection->InintConfig(InConfig);

	InRecordingCollection->StartSpeak();

	TArray<uint8> PCMData;
	int32 IndexCount = 0;
	while (IndexCount < LoopCount)
	{
		IndexCount++;

		int32 Size = 0;
		uint8 *Ptr = InRecordingCollection->GetBuffer(Size);

		int32 Index = PCMData.AddUninitialized(Size);
		FMemory::Memcpy(&PCMData[Index], Ptr, Size);
	}

	InRecordingCollection->StopSpeak();

	TArray<uint8> WAVData;
	SimpleSoundFormatHandle::PCMToWAVByMemory(
		InConfig.Channels,
		InConfig.SampleRate,
		PCMData, 
		WAVData);

	return FFileHelper::SaveArrayToFile(
			WAVData,
			*FPaths::ConvertRelativePathToFull(InSavePath));
}