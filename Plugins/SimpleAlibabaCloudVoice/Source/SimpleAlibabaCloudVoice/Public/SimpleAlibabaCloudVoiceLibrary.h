// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"
#include "SimpleAlibabaCloudVoiceLibrary.generated.h"

UCLASS(meta = (BlueprintThreadSafe, ScriptName = "AlibabaVoiceLibrary"))
class SIMPLEALIBABACLOUDVOICE_API USimpleAlibabaCloudVoiceLibrary : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void SetNlsLogConfig(const FString& InLogPath);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void SetDirectHost(const FString& InIP);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void SetUseSysGetAddrInfo(bool bEnable = true);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void StartWorkThread(int32 InNum = 1);

	// 识别单个音频数据
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void StartSpeechTranscriber(int32& OutHandle,const FNlsInfo &InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, bool bEnableGlobalConfig = true);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void StopSpeechTranscriber(int32 InHandle);

	// 识别单个音频数据
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void SpeechRecognizerFile(const FString& InSpeechPath, const FNlsInfo& InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, bool bEnableGlobalConfig = true);

	// 并发识别多个音频数据
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void SpeechRecognizerMultFile(const TArray<FString>& InSpeechPaths, const FNlsInfo& InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, bool bEnableGlobalConfig = true);

	//语音合成
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static void SpeechSynthesizerFile(const FNlsInfo& InCofigInfo, FNlsCallbackDelegate InCallbackDelegate, const FString& InConent, const FString& InAudioFile, bool bEnableGlobalConfig = true);

	//@brief 获取sendAudio发送延时时间。
	//@param dataSize 待发送数据大小。
	//@param sampleRate 采样率：16k/8K。
	//@param compressRate 数据压缩率，例如压缩比为10:1的16k OPUS编码，此时为10，非压缩数据则为1。
	//@return 返回sendAudio之后需要sleep的时间。
	//@note 对于8k pcm 编码数据, 16位采样，建议每发送1600字节 sleep 100 ms.
	//      对于16k pcm 编码数据, 16位采样，建议每发送3200字节 sleep 100 ms.
	//      对于其它编码格式(OPUS)的数据, 由于传递给SDK的仍然是PCM编码数据,
	//      按照SDK OPUS/OPU 数据长度限制, 需要每次发送640字节 sleep 20ms.
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static int32 GetSendAudioSleepTime(int32 DataSize, int32 SampleRate, int32 CompressRate);

	/**
	 * 根据AccessKey ID和AccessKey Secret重新生成一个token，
	 * 并获取其有效期时间戳
	 */
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static bool GenerateToken(FString& OutToken, int32& OutExpireTime);

	//是否过期
	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static bool IsExpire(int32 InExpireTime);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static bool SimplePlaySoundByFile(const FString& InFilePath);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static bool SimplePlaySoundByMemory(const TArray<uint8> &InSoundPtr);

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static bool SimpleStopSound();

	UFUNCTION(BlueprintCallable, Category = "CloudVoice")
	static bool StartRecording(const FString& InSavePath,float InTime);
};
