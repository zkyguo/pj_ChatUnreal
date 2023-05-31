// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"
#include "SimpleAlibabaCloudVoiceMacro.h"

class SIMPLEALIBABACLOUDVOICE_API FSimpleAlibabaCloudVoiceManage
{
public:
	FSimpleAlibabaCloudVoiceManage();
	~FSimpleAlibabaCloudVoiceManage();

	static FSimpleAlibabaCloudVoiceManage* Get();
	static void Destroy();

public:
	//设置Nls存储路径
	void SetNlsLogConfig(const FString& InLogPath);

	// 私有云部署的情况下可进行直连IP的设置
	// 必须在StartWorkThread()前调用
	void SetDirectHost(const FString& InIP);

	// 存在部分设备在设置了dns后仍然无法通过SDK的dns获取可用的IP,
   // 可调用此接口主动启用系统的getaddrinfo来解决这个问题.
	void SetUseSysGetAddrInfo(bool bEnable = true);

	//启用工作线程
	void StartWorkThread(int32 InNum = 1);

	//@brief 获取sendAudio发送延时时间。
	//@param dataSize 待发送数据大小。
	//@param sampleRate 采样率：16k/8K。
	//@param compressRate 数据压缩率，例如压缩比为10:1的16k OPUS编码，此时为10，非压缩数据则为1。
	//@return 返回sendAudio之后需要sleep的时间。
	//@note 对于8k pcm 编码数据, 16位采样，建议每发送1600字节 sleep 100 ms.
	//      对于16k pcm 编码数据, 16位采样，建议每发送3200字节 sleep 100 ms.
	//      对于其它编码格式(OPUS)的数据, 由于传递给SDK的仍然是PCM编码数据,
	//      按照SDK OPUS/OPU 数据长度限制, 需要每次发送640字节 sleep 20ms.
	int32 GetSendAudioSleepTime(int32 DataSize,int32 SampleRate,int32 CompressRate);
	
	//是否过期
	bool IsExpire(uint32 InExpireTime)const;

	/**
	 * 根据AccessKey ID和AccessKey Secret重新生成一个token，
	 * 并获取其有效期时间戳
	 */
	bool GenerateToken(FString &OutToken,uint32 &OutExpireTime);

	// 识别单个音频数据s
	void StartSpeechTranscriber(int32& OutHandle, const FNlsInfo& InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback, bool bEnableGlobalConfig = true);

	void StopSpeechTranscriber(int32 InHandle);

	// 识别单个音频数据s
	void SpeechRecognizerFile(const FString& InSpeechPath, const FNlsInfo& InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback, bool bEnableGlobalConfig = true);

	// 并发识别多个音频数据
	void SpeechRecognizerMultFile(const TArray<FString>& InSpeechPaths, const FNlsInfo& InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback, bool bEnableGlobalConfig = true);

	// 合成单个文本数据
	void SpeechSynthesizerFile(const FNlsInfo& InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback,const FString &InConent, const FString& InAudioFile,bool bEnableGlobalConfig = true);

protected:
	bool CheckTokenExpireAndGenerateNewToken();

protected:
	// 识别单个音频数据s
	void SpeechTranscriber_Thread(int32 InHandle, const FNlsInfo InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback);
	void SpeechRecognizerFile_Thread(const FString InSpeechPath, const FNlsInfo InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback);
	void SpeechSynthesizerFile_Thread(const FString InConent, const FString InAudioFile,const FNlsInfo InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback);

protected:
	void OnRequestComplete(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, FNlsParamCallBack* ParamCallBack);
	void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived, FNlsParamCallBack* ParamCallBack);
	void OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue, FNlsParamCallBack* ParamCallBack);

protected:
	void OnRequestAudioAddress(const FSpeechSynthesizerResponded &Responded,FNlsParamCallBack* ParamCallBack);

protected:
	void InitLog();

	//SpeechTranscriber
private:
	int32 GetSpeechTranscriberHandle();
	bool RemoveSpeechTranscriberHandle(int32 InHandle);
	bool FindSpeechTranscriberHandle(int32 InHandle);

	bool IsSpeechTranscriberHandleExit(int32 InHandle) const;

	int32 CreateUniqueHandle();

private:
	static FSimpleAlibabaCloudVoiceManage* Manage;

	TMap<int32, bool> SpeechTranscriberPool;
	FCriticalSection Mutex;
};
