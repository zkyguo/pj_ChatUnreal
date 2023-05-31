// Copyright (C) RenZhai.2023.All Rights Reserved.

#ifndef HAVE_STRUCT_TIMESPEC
#define HAVE_STRUCT_TIMESPEC
#endif // HAVE_STRUCT_TIMESPEC

#if PLATFORM_WINDOWS
#pragma optimize("",off) 
#endif

#include "SimpleAlibabaCloudVoiceManage.h"
#include "SimpleAlibabaCloudVoiceLog.h"
#include "SimpleAlibabaCloudVoiceConfig.h"
#include "SimpleAlibabaCloudVoiceMacro.h"
#include "JSON/SimpleAlibabaCluoudVoiceJson.h"
#include "Sound/SimpleSoundHandle.h"
#include "Sound/SimpleRecordingCollection.h"

#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/PreWindowsApi.h"

#include "nlsClient.h"
#include "nlsEvent.h"
#include "nlsToken.h"
#include "speechRecognizerRequest.h"
#include "speechTranscriberRequest.h"
#include "SpeechSynthesizerRequest.h"

#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <playsoundapi.h>

using AlibabaNls::NlsClient;
using AlibabaNls::LogDebug;
using AlibabaNls::LogInfo;
using AlibabaNls::LogError;
using AlibabaNls::LogWarning;

using AlibabaNls::SpeechTranscriberRequest;
using AlibabaNls::SpeechRecognizerRequest;
using AlibabaNls::SpeechSynthesizerRequest;
using AlibabaNls::NlsEvent;

struct FSpeechSynthesizerRequestStage
{
	static FString RequestMergeStage;//POST
	static FString RequestAudioAddress;//GET
	static FString RequestDownloadAudio;//请求下载
};

FString FSpeechSynthesizerRequestStage::RequestMergeStage = TEXT("RequestMergeStage");
FString FSpeechSynthesizerRequestStage::RequestAudioAddress = TEXT("RequestAudioAddress");
FString FSpeechSynthesizerRequestStage::RequestDownloadAudio = TEXT("RequestDownloadAudio");

struct FNlsAbandonable :FNonAbandonableTask
{
	FNlsAbandonable(const FSimpleDelegate& InThreadDelegate)
		:ThreadDelegate(InThreadDelegate)
	{}

	~FNlsAbandonable()
	{

	}

	void DoWork()
	{
		ThreadDelegate.ExecuteIfBound();
	}

	//ID
	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FNlsAbandonable, STATGROUP_ThreadPoolAsyncTasks);
	}

protected:
	FSimpleDelegate ThreadDelegate;
};

FSimpleAlibabaCloudVoiceManage* FSimpleAlibabaCloudVoiceManage::Manage = NULL;

FSimpleAlibabaCloudVoiceManage::FSimpleAlibabaCloudVoiceManage()
{

}

FSimpleAlibabaCloudVoiceManage::~FSimpleAlibabaCloudVoiceManage()
{
	SpeechTranscriberPool.Empty();
	AlibabaNls::NlsClient::releaseInstance();
}

FSimpleAlibabaCloudVoiceManage* FSimpleAlibabaCloudVoiceManage::Get()
{
	if (!Manage)
	{
		Manage = new FSimpleAlibabaCloudVoiceManage();

		Manage->InitLog();
	}

	return Manage;
}

void FSimpleAlibabaCloudVoiceManage::Destroy()
{
	if (Manage)
	{
		delete Manage;		
	}

	Manage = NULL;
}

FString GetMsgTypeString(AlibabaNls::NlsEvent::EventType InType)
{
	switch (InType) 
	{
	case AlibabaNls::NlsEvent::EventType::TaskFailed:
		return TEXT("TaskFailed");
	case AlibabaNls::NlsEvent::EventType::RecognitionStarted:
		return TEXT("RecognitionStarted");
	case AlibabaNls::NlsEvent::EventType::RecognitionCompleted:
		return TEXT("RecognitionCompleted");
	case AlibabaNls::NlsEvent::EventType::RecognitionResultChanged:
		return TEXT("RecognitionResultChanged");
	case AlibabaNls::NlsEvent::EventType::WakeWordVerificationCompleted:
		return TEXT("WakeWordVerificationCompleted");
	case AlibabaNls::NlsEvent::EventType::TranscriptionStarted:
		return TEXT("TranscriptionStarted");
	case AlibabaNls::NlsEvent::EventType::SentenceBegin:
		return TEXT("SentenceBegin");
	case AlibabaNls::NlsEvent::EventType::TranscriptionResultChanged:
		return TEXT("TranscriptionResultChanged");
	case AlibabaNls::NlsEvent::EventType::SentenceEnd:
		return TEXT("SentenceEnd");
	case AlibabaNls::NlsEvent::EventType::SentenceSemantics:
		return TEXT("SentenceSemantics");
	case AlibabaNls::NlsEvent::EventType::TranscriptionCompleted:
		return TEXT("TranscriptionCompleted");
	case AlibabaNls::NlsEvent::EventType::SynthesisStarted:
		return TEXT("SynthesisStarted");
	case AlibabaNls::NlsEvent::EventType::SynthesisCompleted:
		return TEXT("SynthesisCompleted");
	case AlibabaNls::NlsEvent::EventType::Binary:
		return TEXT("Binary");
	case AlibabaNls::NlsEvent::EventType::MetaInfo:
		return TEXT("MetaInfo");
	case AlibabaNls::NlsEvent::EventType::DialogResultGenerated:
		return TEXT("DialogResultGenerated");
	case AlibabaNls::NlsEvent::EventType::Close:
		return TEXT("Close");
	}

	return TEXT("Unknown");
}

//@brief 调用start()，成功与云端建立连接，SDK内部线程上报started事件。
//@param InEvent 回调事件结构，详见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL。可以根据需求自定义参数。
void OnStarted(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack *InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: %s"), *OnEventName, *InCallbackParam->ID.ToString());

	NLS_LOG_PRINT(TEXT("%s: all response=%s"), 
		*OnEventName,
		UTF8_TO_TCHAR(InEvent->getAllResponse()));

	InCallbackParam->Type = ENlsParamCallBackType::CALLBACK_SUCCESS;

	InCallbackParam->Event->Trigger();

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

//@brief 识别结果发生了变化，SDK在接收到云端返回的最新结果时，其内部线程上报ResultChanged事件。
//@param InEvent 回调事件结构，详情参见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnResultChanged(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: %s"),*OnEventName,*InCallbackParam->ID.ToString());

	//使用用户自定义参数示例。
	NLS_LOG_PRINT(TEXT("%s: status code=%d, task id=%s, index=%d, time=%d, begin_time=%d, result=%s"),
		*OnEventName,
		InEvent->getStatusCode(),
		UTF8_TO_TCHAR(InEvent->getTaskId()),
		InEvent->getSentenceIndex(), //句子编号，从1开始递增。
		InEvent->getSentenceTime(), //当前已处理的音频时长，单位：毫秒。
		InEvent->getSentenceBeginTime(), // 对应的SentenceBegin事件的时间。
		UTF8_TO_TCHAR(InEvent->getResult())); // 当前句子的完整识别结果。

	NLS_LOG_PRINT(TEXT("%s: all response=%s"),
		*OnEventName,
		UTF8_TO_TCHAR(InEvent->getAllResponse()));

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

//@brief 服务端停止实时音频流识别时，SDK内部线程上报Completed事件。
//@note 上报Completed事件之后，SDK内部会关闭识别连接通道。此时调用sendAudio会返回负值，请停止发送。
//@param InEvent 回调事件结构，详情参见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnCompleted(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: %s"), 
		*OnEventName,
		*InCallbackParam->ID.ToString());

	//使用用户自定义参数示例。
	NLS_LOG_PRINT(TEXT("%s: status code=%d, task id=%s"),
		*OnEventName,
		InEvent->getStatusCode(),
		UTF8_TO_TCHAR(InEvent->getTaskId()));

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);

	if (InEvent->getMsgType() == AlibabaNls::NlsEvent::EventType::SynthesisCompleted)
	{
		if (InCallbackParam->BinaryData.Num() > 0)
		{
			FFileHelper::SaveArrayToFile(
				InCallbackParam->BinaryData,
				*InCallbackParam->AudioFullFile);
		}
	}
}

//@brief 服务端检测到了一句话的开始，SDK内部线程上报SentenceBegin事件。
//@param InEvent 回调事件结构，详情参见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnSentenceBegin(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: %s"), 
		*OnEventName,
		*InCallbackParam->ID.ToString());

	// 演示如何打印/使用用户自定义参数示例。
	NLS_LOG_PRINT(TEXT("%s: status code=%d, task id=%s, index=%d, time=%d"),
		*OnEventName,
		InEvent->getStatusCode(),
		UTF8_TO_TCHAR(InEvent->getTaskId()),
		InEvent->getSentenceIndex(), //句子编号，从1开始递增。
		InEvent->getSentenceTime());//当前已处理的音频时长，单位：毫秒。

	NLS_LOG_PRINT(TEXT("%s: all response=%s"), 
		*OnEventName,
		UTF8_TO_TCHAR(InEvent->getAllResponse()));

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

//@brief 服务端检测到了一句话结束，SDK内部线程上报SentenceEnd事件。
//@param InEvent 回调事件结构，详情参见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnSentenceEnd(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: %s"),
		*OnEventName,
		*InCallbackParam->ID.ToString());

	//使用用户自定义参数示例。
	NLS_LOG_PRINT(TEXT("%s: status code=%d, task id=%s, index=%d, time=%d, begin_time=%d, result=%s"),
		*OnEventName,
		InEvent->getStatusCode(),
		UTF8_TO_TCHAR(InEvent->getTaskId()),
		InEvent->getSentenceIndex(), //句子编号，从1开始递增。
		InEvent->getSentenceTime(), //当前已处理的音频时长，单位：毫秒。
		InEvent->getSentenceBeginTime(), // 对应的SentenceBegin事件的时间。
		UTF8_TO_TCHAR(InEvent->getResult())); // 当前句子的完整识别结果。

	NLS_LOG_PRINT(TEXT("%s: all response=%s"),
		*OnEventName,
		UTF8_TO_TCHAR(InEvent->getAllResponse()));

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

void OnTaskFailed(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	if (InCallbackParam->Handle != INDEX_NONE)
	{
		FSimpleAlibabaCloudVoiceManage::Get()->StopSpeechTranscriber(InCallbackParam->Handle);
	}

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_ERROR_PRINT(TEXT("%s: %s"), 
		*OnEventName,
		*InCallbackParam->ID.ToString());

	NLS_ERROR_PRINT(TEXT("%s: Code = %i, ErrorMsg = %s"),
		*OnEventName,
		InEvent->getStatusCode(),
		UTF8_TO_TCHAR(InEvent->getErrorMessage()));

	NLS_ERROR_PRINT(TEXT("%s: all response=%s"),
		*OnEventName,
	UTF8_TO_TCHAR(InEvent->getAllResponse()));

	InCallbackParam->Type = ENlsParamCallBackType::CALLBACK_FAIL;

	//InCallbackParam->Event->Trigger();

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

//@brief 识别结束或发生异常时，会关闭连接通道，SDK内部线程上报ChannelCloseed事件。
//@param InEvent 回调事件结构，详情参见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnChannelClosed(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: %s"),*OnEventName, *InCallbackParam->ID.ToString());

	NLS_LOG_PRINT(TEXT("%s: all response=%s"),  *OnEventName ,UTF8_TO_TCHAR(InEvent->getAllResponse()));

	InCallbackParam->Type = ENlsParamCallBackType::CALLBACK_CHANNEL_CLOSED_SUCCESS;

	InCallbackParam->Event->Trigger();

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

void OnMessage(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s: all response=%s"),
		*OnEventName,
		UTF8_TO_TCHAR(InEvent->getAllResponse()));
}

//@brief 返回tts文本对应的日志信息，增量返回对应的字幕信息。
//@param InEvent 回调事件结构，详见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnMetaInfo(NlsEvent* InEvent, void* InParam)
{
	FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

	FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

	NLS_LOG_PRINT(TEXT("%s:AudioFullFile: %s"),
		*OnEventName, 
		*InCallbackParam->AudioFullFile);

	NLS_LOG_PRINT(TEXT("%s: all response=%s"),
		*OnEventName,
		UTF8_TO_TCHAR(InEvent->getAllResponse()));

	DEFINE_UE_NLSEVENT(UENlsEvent);
	CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
}

//@brief 文本上报服务端后，收到服务端返回的二进制音频数据，
// SDK内部线程通过BinaryDataRecved事件上报给用户。
//@param InEvent 回调事件结构，详见nlsEvent.h。
//@param InParam 回调自定义参数，默认为NULL，可以根据需求自定义参数。
void OnBinaryDataRecved(NlsEvent* InEvent, void* InParam)
{
	if (InEvent)
	{
		FNlsParamCallBack* InCallbackParam = (FNlsParamCallBack*)InParam;

		// getBinaryData() ：获取文本合成的二进制音频数据。
		std::vector<unsigned char> TmpBinaryData = InEvent->getBinaryData();

		if (TmpBinaryData.size() > 0)
		{
			int32 Size = TmpBinaryData.size() * sizeof(uint8);
			int32 Index = InCallbackParam->BinaryData.Num() * sizeof(uint8);

			//分配空间
			InCallbackParam->BinaryData.AddUninitialized(Size);
		
			FMemory::Memzero(&InCallbackParam->BinaryData[Index], Size);
			FMemory::Memcpy(&InCallbackParam->BinaryData[Index], TmpBinaryData.data(), Size);
		}

		//模拟销毁
		TmpBinaryData.clear();
		TmpBinaryData.shrink_to_fit();

		FString OnEventName = GetMsgTypeString(InEvent->getMsgType());

		NLS_LOG_PRINT(TEXT("%s: all ErrorMessage=%s"),
			*OnEventName,
			UTF8_TO_TCHAR(InEvent->getErrorMessage()));

		NLS_LOG_PRINT(TEXT("%s: all response=%s"),
			*OnEventName,
			UTF8_TO_TCHAR(InEvent->getAllResponse()));
	}
}

void FSimpleAlibabaCloudVoiceManage::InitLog()
{
	//配置初始化
	FSimpleAlibabaCloudVoiceConfig::Get()->Init();
	
	SetNlsLogConfig(FSimpleAlibabaCloudVoiceConfig::Get()->LogConfig.NlsPath);
}

void FSimpleAlibabaCloudVoiceManage::SetNlsLogConfig(const FString& InLogPath)
{
	uint32 LogFileSize = FSimpleAlibabaCloudVoiceConfig::Get()->LogConfig.LogFileSize;
	uint32 LogFileNum = FSimpleAlibabaCloudVoiceConfig::Get()->LogConfig.LogFileNum;
	
	std::string InLogPathString = TCHAR_TO_UTF8(*InLogPath);

	NlsClient::getInstance()->setLogConfig(InLogPathString.c_str(),
		LogError, LogFileSize, LogFileNum);

	NlsClient::getInstance()->setLogConfig(InLogPathString.c_str(),
		LogDebug, LogFileSize, LogFileNum);

	NlsClient::getInstance()->setLogConfig(InLogPathString.c_str(),
		LogWarning, LogFileSize, LogFileNum);

	NlsClient::getInstance()->setLogConfig(InLogPathString.c_str(),
		LogInfo, LogFileSize, LogFileNum);
}

void FSimpleAlibabaCloudVoiceManage::SetDirectHost(const FString& InIP)
{
	NlsClient::getInstance()->setDirectHost(TCHAR_TO_UTF8(*InIP));
}

void FSimpleAlibabaCloudVoiceManage::SetUseSysGetAddrInfo(bool bEnable)
{
	NlsClient::getInstance()->setUseSysGetAddrInfo(bEnable);
}

void FSimpleAlibabaCloudVoiceManage::StartWorkThread(int32 InNum)
{
	NlsClient::getInstance()->startWorkThread(InNum);
}

int32 FSimpleAlibabaCloudVoiceManage::GetSendAudioSleepTime(int32 DataSize, int32 SampleRate, int32 CompressRate)
{
	// 仅支持16位采样
	const int SampleBytes = 16;     // 仅支持单通道
	const int SoundChannel = 1;     // 当前采样率，采样位数下每秒采样数据的大小。

	// 当前采样率，采样位数下每毫秒采样数据的大小。
	int Bytes = (SampleRate * SampleBytes * SoundChannel) / 8;     
	// 待发送数据大小除以每毫秒采样数据大小，以获取sleep时间。
	int BytesMs = Bytes / 1000;

	int SleepMs = (DataSize * CompressRate) / BytesMs;
	return SleepMs;
}

// 识别单个音频数据s
void FSimpleAlibabaCloudVoiceManage::StartSpeechTranscriber(
	int32& OutHandle,
	const FNlsInfo& InCofigInfo, 
	TFunction<void(const FNlsEvent&)> InProgressCallback, 
	bool bEnableGlobalConfig)
{
	if (CheckTokenExpireAndGenerateNewToken())
	{
		OutHandle = GetSpeechTranscriberHandle();
		if (bEnableGlobalConfig)
		{
			(new FAutoDeleteAsyncTask<FNlsAbandonable>(
				FSimpleDelegate::CreateRaw(this, &FSimpleAlibabaCloudVoiceManage::SpeechTranscriber_Thread,
					OutHandle, FSimpleAlibabaCloudVoiceConfig::Get()->Info, InProgressCallback)))
				->StartBackgroundTask();
		}
		else
		{
			(new FAutoDeleteAsyncTask<FNlsAbandonable>(
				FSimpleDelegate::CreateRaw(this, &FSimpleAlibabaCloudVoiceManage::SpeechTranscriber_Thread,
					OutHandle, InCofigInfo, InProgressCallback)))
				->StartBackgroundTask();
		}
	}
	else
	{
		NLS_ERROR_PRINT(TEXT("The key or account ID is incorrect."));
	}
}

void FSimpleAlibabaCloudVoiceManage::StopSpeechTranscriber(int32 InHandle)
{
	FScopeLock ScopeLock(&Mutex);

	if (bool* InValue = SpeechTranscriberPool.Find(InHandle))
	{
		*InValue = true;
	}
}

void FSimpleAlibabaCloudVoiceManage::SpeechTranscriber_Thread(int32 InHandle,const FNlsInfo InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback)
{
	if (IsSpeechTranscriberHandleExit(InHandle))
	{
		FNlsParamCallBack* ParamCallBack = new FNlsParamCallBack();
		ParamCallBack->ProgressCallback = InProgressCallback;
		ParamCallBack->Handle = InHandle;

		if (SpeechTranscriberRequest* Request =
			NlsClient::getInstance()->createTranscriberRequest())
		{
			// 设置识别启动回调函数
			Request->setOnTranscriptionStarted(OnStarted, ParamCallBack);

			// 设置识别结果变化回调函数
			Request->setOnTranscriptionResultChanged(OnResultChanged, ParamCallBack);

			// 设置语音转写结束回调函数
			Request->setOnTranscriptionCompleted(OnCompleted, ParamCallBack);

			// 设置一句话开始回调函数
			Request->setOnSentenceBegin(OnSentenceBegin, ParamCallBack);

			// 设置一句话结束回调函数
			Request->setOnSentenceEnd(OnSentenceEnd, ParamCallBack);

			// 设置异常识别回调函数
			Request->setOnTaskFailed(OnTaskFailed, ParamCallBack);

			// 设置识别通道关闭回调函数
			Request->setOnChannelClosed(OnChannelClosed, ParamCallBack);

			// 设置appKey。必填参数。
			Request->setAppKey(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.AppKey));

			// 设置账号校验token, 必填参数
			Request->setToken(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token));

			// 设置音频数据编码格式。可选参数，目前支持PCM/OPUS，默认为PCM。
			Request->setFormat(TCHAR_TO_UTF8(*InCofigInfo.GetEncodingFormatString()));

			// 必须设置 否则是乱码
			Request->setOutputFormat(TCHAR_TO_UTF8(*InCofigInfo.OutputFormat));

			// 设置音频数据采样率。可选参数，目前支持16000/8000。默认为16000。
			Request->setSampleRate(InCofigInfo.SampleRate);

			// 设置是否返回中间识别结果。可选参数，默认false。
			Request->setIntermediateResult(InCofigInfo.bIntermediateResult);

			// 设置是否在后处理中添加标点。可选参数，默认false。
			Request->setPunctuationPrediction(InCofigInfo.bPunctuationPrediction);

			// 设置是否在后处理中执行ITN。可选参数，默认false。
			Request->setInverseTextNormalization(InCofigInfo.bInverseTextNormalization);

			// 语音断句检测阈值，一句话之后静音长度超过该值，即本句结束，合法参数范围200ms～2000ms，默认值800ms。
			Request->setMaxSentenceSilence(InCofigInfo.MaxSentenceSilence);

			// 定制语言模型id，可选。
			if (!InCofigInfo.CustomizationId.IsEmpty())
			{
				Request->setCustomizationId(TCHAR_TO_UTF8(*InCofigInfo.CustomizationId));
			}

			// 定制泛热词id，可选。
			if (!InCofigInfo.VocabularyId.IsEmpty())
			{
				Request->setVocabularyId(TCHAR_TO_UTF8(*InCofigInfo.VocabularyId));
			}
			// 用于传递某些定制化、高级参数设置，参数格式为JSON格式：{"key": "value"}。
			if (!InCofigInfo.PayloadParam.IsEmpty())
			{
				Request->setPayloadParam(TCHAR_TO_UTF8(*InCofigInfo.PayloadParam));
			}

			int32 ReturnValue = Request->start();
			if (ReturnValue < 0)
			{
				NLS_ERROR_PRINT(TEXT("start() failed. may be can not connect server. please check network or firewalld"));

				// start()失败，释放request对象。
				NlsClient::getInstance()->releaseTranscriberRequest(Request);
				delete ParamCallBack;

				return;
			}
			else
			{
				NLS_LOG_PRINT(TEXT("wait started callback."));

				ParamCallBack->Event->Wait(10000);
				if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_TIMEOUT ||
					ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_FAIL)
				{
					NLS_ERROR_PRINT(TEXT("start timeout."));
					Request->cancel();

					NlsClient::getInstance()->releaseTranscriberRequest(Request);
					delete ParamCallBack;

					return;
				}
				else if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_SUCCESS)
				{
					TSharedPtr<FSimpleRecordingCollection> RecordingCollection = FSimpleRecordingCollection::CreateRecordingCollection();
					
					FRecordingConfig InConfig;
					InConfig.SampleRate = InCofigInfo.SampleRate;

					RecordingCollection->InintConfig(InConfig);

					//开始录声音
					RecordingCollection->StartSpeak();

					bool bBreak = FindSpeechTranscriberHandle(InHandle);
					while (!bBreak)
					{
						bBreak = FindSpeechTranscriberHandle(InHandle);

						int32 DataSize = 0;
						uint8* Ptr = RecordingCollection->GetBuffer(DataSize);

						int32 CurrentIndex = 0;
						while (CurrentIndex < DataSize)
						{
							uint8 TmpData[FRAME_SIZE] = { 0 };

							int32 Remaining = DataSize - CurrentIndex;
							if (Remaining > FRAME_SIZE)
							{
								Remaining = FRAME_SIZE;
							}

							FMemory::Memcpy(TmpData, Ptr, Remaining);

							Ptr += Remaining;
							CurrentIndex += Remaining;

							ReturnValue = Request->sendAudio(TmpData, Remaining);

							if (ReturnValue < 0)
							{
								NLS_ERROR_PRINT(TEXT("send data fail."));
								bBreak = true;

								break;
							}

							// 语音数据发送控制：
							// 语音数据是实时的，不用sleep控制速率，直接发送即可。
							// 语音数据来自文件（也即本示例代码模拟的语音流发送机制），发送时需要控制速率，使单位时间内发送的数据大小接近单位时间原始语音数据存储的大小。

							// 根据发送数据大小、采样率、数据压缩比来获取sleep时间。
							//int32 SleepMs = GetSendAudioSleepTime(Remaining, InCofigInfo.SampleRate, 1);
							//
							////语音数据发送延时控制
							//Sleep(SleepMs);
						}
					}

					RecordingCollection->StopSpeak();
				}

				NLS_LOG_PRINT(TEXT("sendAudio done."));

				//通知云端数据发送结束
				//stop()为异步操作。失败返回TaskFailed事件。

				ReturnValue = Request->stop();
				if (ReturnValue == 0)
				{
					NLS_LOG_PRINT(TEXT("wait closed callback."));

					// 语音服务器存在来不及处理当前请求, 10s内不返回任何回调的问题,
					// 然后在10s后返回一个TaskFailed回调, 错误信息为:
					// "Gateway:IDLE_TIMEOUT:Websocket session is idle for too long time, the last directive is 'StopRecognition'!"
					// 所以需要设置一个超时机制.
					ParamCallBack->Type = ENlsParamCallBackType::CALLBACK_TIMEOUT;
					ParamCallBack->Event->Wait(10000);
					if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_TIMEOUT)
					{
						NLS_ERROR_PRINT(TEXT("Stop timeout."));
						Request->cancel();

						NlsClient::getInstance()->releaseTranscriberRequest(Request);
						delete ParamCallBack;

						return;
					}
				}
				else
				{
					NLS_ERROR_PRINT(TEXT("stop ret is %d"), ReturnValue);
				}

				NlsClient::getInstance()->releaseTranscriberRequest(Request);
				delete ParamCallBack;

				return;
			}
		}
		else
		{
			NLS_ERROR_PRINT(TEXT("createTranscriberRequest failed"));

			delete ParamCallBack;
		}

		RemoveSpeechTranscriberHandle(InHandle);
	}
}

void FSimpleAlibabaCloudVoiceManage::SpeechRecognizerFile_Thread(const FString InSpeechPath, const FNlsInfo InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback)
{
	if (!InSpeechPath.IsEmpty())
	{
		FNlsParamCallBack* ParamCallBack = new FNlsParamCallBack();
		ParamCallBack->ProgressCallback = InProgressCallback;

		TArray<uint8> FileData;
		FFileHelper::LoadFileToArray(FileData, *InSpeechPath);

		if (FileData.Num() > 0)
		{
			if (SpeechRecognizerRequest* Request =
				NlsClient::getInstance()->createRecognizerRequest())
			{
				// 设置识别启动回调函数
				Request->setOnRecognitionStarted(OnStarted, ParamCallBack);

				// 设置识别结果变化回调函数
				Request->setOnRecognitionResultChanged(OnResultChanged, ParamCallBack);

				// 设置语音转写结束回调函数
				Request->setOnRecognitionCompleted(OnCompleted, ParamCallBack);

				// 设置异常识别回调函数
				Request->setOnTaskFailed(OnTaskFailed, ParamCallBack);

				// 设置识别通道关闭回调函数
				Request->setOnChannelClosed(OnChannelClosed, ParamCallBack);

				// 设置appKey。必填参数。
				Request->setAppKey(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.AppKey));

				// 设置账号校验token, 必填参数
				Request->setToken(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token));

				// 设置音频数据编码格式。可选参数，目前支持PCM/OPUS，默认为PCM。
				Request->setFormat(TCHAR_TO_UTF8(*InCofigInfo.GetEncodingFormatString()));

				// 必须设置 否则是乱码
				Request->setOutputFormat(TCHAR_TO_UTF8(*InCofigInfo.OutputFormat));

				// 设置音频数据采样率。可选参数，目前支持16000/8000。默认为16000。
				Request->setSampleRate(InCofigInfo.SampleRate);

				// 设置是否返回中间识别结果。可选参数，默认false。
				Request->setIntermediateResult(InCofigInfo.bIntermediateResult);

				// 设置是否在后处理中添加标点。可选参数，默认false。
				Request->setPunctuationPrediction(InCofigInfo.bPunctuationPrediction);

				// 设置是否在后处理中执行ITN。可选参数，默认false。
				Request->setInverseTextNormalization(InCofigInfo.bInverseTextNormalization);

				/* 是否启动语音检测, 可选, 默认是False */
				Request->setEnableVoiceDetection(true);
				
				Request->setEnableVoiceDetection(InCofigInfo.bEnableVoiceDetection);
				if (InCofigInfo.bEnableVoiceDetection)
				{
					/*
					 * 允许的最大开始静音, 可选, 单位是毫秒,
					 * 超出后服务端将会发送RecognitionCompleted事件, 结束本次识别.
					 * 注意: 需要先设置enable_voice_detection为true
					 */
					Request->setMaxStartSilence(InCofigInfo.MaxStartSilence);

					/*
					 * 允许的最大结束静音, 可选, 单位是毫秒,
					 * 超出后服务端将会发送RecognitionCompleted事件, 结束本次识别.
					 * 注意: 需要先设置enable_voice_detection为true
					 */
					Request->setMaxEndSilence(InCofigInfo.MaxEndSilence);
				}
				
				// 定制语言模型id，可选。
				if (!InCofigInfo.CustomizationId.IsEmpty())
				{
					Request->setCustomizationId(TCHAR_TO_UTF8(*InCofigInfo.CustomizationId));
				}

				// 定制泛热词id，可选。
				if (!InCofigInfo.VocabularyId.IsEmpty())
				{
					Request->setVocabularyId(TCHAR_TO_UTF8(*InCofigInfo.VocabularyId));
				}
				// 用于传递某些定制化、高级参数设置，参数格式为JSON格式：{"key": "value"}。
				if (!InCofigInfo.PayloadParam.IsEmpty())
				{
					Request->setPayloadParam(TCHAR_TO_UTF8(*InCofigInfo.PayloadParam));
				}

				int32 ReturnValue = Request->start();
				if (ReturnValue < 0)
				{
					NLS_ERROR_PRINT(TEXT("start() failed. may be can not connect server. please check network or firewalld"));

					// start()失败，释放request对象。
					NlsClient::getInstance()->releaseRecognizerRequest(Request);
					delete ParamCallBack;

					return;
				}
				else
				{
					NLS_LOG_PRINT(TEXT("wait started callback."));

					ParamCallBack->Event->Wait(10000);
					if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_TIMEOUT ||
						ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_FAIL)
					{
						NLS_ERROR_PRINT(TEXT("start timeout."));
						Request->cancel();

						NlsClient::getInstance()->releaseRecognizerRequest(Request);
						delete ParamCallBack;

						return;
					}
					else if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_SUCCESS)
					{
						uint8* Ptr = FileData.GetData();
						int32 DataSize = FileData.Num() * sizeof(uint8);

						int32 CurrentIndex = 0;
						while (CurrentIndex < DataSize)
						{
							uint8 TmpData[FRAME_SIZE] = { 0 };

							int32 Remaining = DataSize - CurrentIndex;
							if (Remaining > FRAME_SIZE)
							{
								Remaining = FRAME_SIZE;
							}

							FMemory::Memcpy(TmpData, Ptr, Remaining);

							Ptr += Remaining;
							CurrentIndex += Remaining;

							ReturnValue = Request->sendAudio(TmpData, Remaining);

							if (ReturnValue < 0)
							{
								NLS_ERROR_PRINT(TEXT("send data fail."));
								break;
							}

							// 语音数据发送控制：
							// 语音数据是实时的，不用sleep控制速率，直接发送即可。
							// 语音数据来自文件（也即本示例代码模拟的语音流发送机制），发送时需要控制速率，使单位时间内发送的数据大小接近单位时间原始语音数据存储的大小。

							// 根据发送数据大小、采样率、数据压缩比来获取sleep时间。
							int32 SleepMs = GetSendAudioSleepTime(Remaining, InCofigInfo.SampleRate, 1);

							//语音数据发送延时控制
							Sleep(SleepMs);
						}
					}

					NLS_LOG_PRINT(TEXT("sendAudio done."));

					//通知云端数据发送结束
					//stop()为异步操作。失败返回TaskFailed事件。
					ReturnValue = Request->stop();
					if (ReturnValue == 0)
					{
						NLS_LOG_PRINT(TEXT("wait closed callback."));

						// 语音服务器存在来不及处理当前请求, 10s内不返回任何回调的问题,
						// 然后在10s后返回一个TaskFailed回调, 错误信息为:
						// "Gateway:IDLE_TIMEOUT:Websocket session is idle for too long time, the last directive is 'StopRecognition'!"
						// 所以需要设置一个超时机制.
						ParamCallBack->Type = ENlsParamCallBackType::CALLBACK_TIMEOUT;
						ParamCallBack->Event->Wait(10000);
						if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_TIMEOUT)
						{
							NLS_ERROR_PRINT(TEXT("Stop timeout."));
							Request->cancel();

							NlsClient::getInstance()->releaseRecognizerRequest(Request);
							delete ParamCallBack;

							return;
						}
					}
					else
					{
						NLS_ERROR_PRINT(TEXT("stop ret is %d"), ReturnValue);
					}

					NlsClient::getInstance()->releaseRecognizerRequest(Request);
					delete ParamCallBack;

					return;
				}
			}
			else
			{
				NLS_ERROR_PRINT(TEXT("createTranscriberRequest failed"));

				delete ParamCallBack;
			}
		}
		else
		{
			NLS_ERROR_PRINT(TEXT("[%s] isn't exist.."), *InSpeechPath);
		}
	}
}

void FSimpleAlibabaCloudVoiceManage::SpeechRecognizerFile(const FString& InSpeechPath, const FNlsInfo& InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback, bool bEnableGlobalConfig)
{
	if (CheckTokenExpireAndGenerateNewToken())
	{
		if (bEnableGlobalConfig)
		{
			(new FAutoDeleteAsyncTask<FNlsAbandonable>(
				FSimpleDelegate::CreateRaw(this, &FSimpleAlibabaCloudVoiceManage::SpeechRecognizerFile_Thread,
					InSpeechPath, FSimpleAlibabaCloudVoiceConfig::Get()->Info, InProgressCallback)))
				->StartBackgroundTask();
		}
		else
		{
			(new FAutoDeleteAsyncTask<FNlsAbandonable>(
				FSimpleDelegate::CreateRaw(this, &FSimpleAlibabaCloudVoiceManage::SpeechRecognizerFile_Thread,
					InSpeechPath, InCofigInfo, InProgressCallback)))
				->StartBackgroundTask();
		}
	}
	else
	{
		NLS_ERROR_PRINT(TEXT("The key or account ID is incorrect."));
	}
}

// 并发识别多个音频数据
void FSimpleAlibabaCloudVoiceManage::SpeechRecognizerMultFile(const TArray<FString>& InSpeechPaths, const FNlsInfo& InCofigInfo, TFunction<void(const FNlsEvent&)> InProgressCallback, bool bEnableGlobalConfig)
{
	for (auto& Tmp : InSpeechPaths)
	{
		SpeechRecognizerFile(Tmp, InCofigInfo, InProgressCallback, bEnableGlobalConfig);
	}
}

void FSimpleAlibabaCloudVoiceManage::SpeechSynthesizerFile(
	const FNlsInfo& InCofigInfo, 
	TFunction<void(const FNlsEvent&)> InProgressCallback, 
	const FString& InConent,
	const FString& InAudioFile, 
	bool bEnableGlobalConfig)
{
	if (CheckTokenExpireAndGenerateNewToken())
	{
		if (bEnableGlobalConfig)
		{
			(new FAutoDeleteAsyncTask<FNlsAbandonable>(
				FSimpleDelegate::CreateRaw(this, &FSimpleAlibabaCloudVoiceManage::SpeechSynthesizerFile_Thread,
					InConent, InAudioFile,
					FSimpleAlibabaCloudVoiceConfig::Get()->Info,
					InProgressCallback)))
				->StartBackgroundTask();
		}
		else
		{
			(new FAutoDeleteAsyncTask<FNlsAbandonable>(
				FSimpleDelegate::CreateRaw(this, &FSimpleAlibabaCloudVoiceManage::SpeechSynthesizerFile_Thread,
					InConent,
					InAudioFile,
					InCofigInfo,
					InProgressCallback)))
				->StartBackgroundTask();
		}
	}
	else
	{
		NLS_ERROR_PRINT(TEXT("The key or account ID is incorrect."));
	}
}

void FSimpleAlibabaCloudVoiceManage::SpeechSynthesizerFile_Thread(
	const FString InConent,
	const FString InAudioFile,
	const FNlsInfo InCofigInfo, 
	TFunction<void(const FNlsEvent&)> InProgressCallback)
{
	if (!InConent.IsEmpty())
	{
		FNlsParamCallBack* ParamCallBack = new FNlsParamCallBack();
		ParamCallBack->ProgressCallback = InProgressCallback;
		ParamCallBack->AudioFullFile = FPaths::ConvertRelativePathToFull(InAudioFile);
		ParamCallBack->bAutomaticallyPlaySound = InCofigInfo.bAutomaticallyPlaySound;

		// 创建语音识别SpeechSynthesizerRequest对象.
		// 默认为实时短文本语音合成请求, 支持一次性合成300字符以内的文字,
		// 其中1个汉字、1个英文字母或1个标点均算作1个字符,
		// 超过300个字符的内容将会报错(或者截断).
		// 一次性合成超过300字符可考虑长文本语音合成功能.
		AlibabaNls::TtsVersion Version = AlibabaNls::ShortTts;
		int32 CharsCount = NlsClient::getInstance()->calculateUtf8Chars(TCHAR_TO_UTF8(*InConent));
		if (CharsCount > 300) 
		{
			Version = AlibabaNls::LongTts;
		}

		if (0)
		{
			if (SpeechSynthesizerRequest* Request =
				NlsClient::getInstance()->createSynthesizerRequest(Version))
			{
				// 设置音频合成结束回调函数
				Request->setOnSynthesisCompleted(OnCompleted, ParamCallBack);

				// 设置异常失败回调函数
				Request->setOnTaskFailed(OnTaskFailed, ParamCallBack);

				// 设置音频合成通道关闭回调函数
				Request->setOnChannelClosed(OnChannelClosed, ParamCallBack);

				// 设置文本音频数据接收回调函数
				Request->setOnBinaryDataReceived(OnBinaryDataRecved, ParamCallBack);

				// 设置字幕信息
				Request->setOnMetaInfo(OnMetaInfo, ParamCallBack);
				//Request->setOnMessage(OnMessage, ParamCallBack);

				// 设置appKey。必填参数。
				Request->setAppKey(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.AppKey));

				// 设置账号校验token, 必填参数
				Request->setToken(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token));

				// 设置音频数据编码格式。可选参数，目前支持PCM/OPUS，默认为PCM。
				//Request->setFormat(TCHAR_TO_UTF8(*InCofigInfo.GetEncodingFormatString()));

				// 必须设置 否则是乱码
				Request->setOutputFormat(TCHAR_TO_UTF8(*InCofigInfo.OutputFormat));

				// 设置待合成文本, 必填参数. 文本内容必须为UTF-8编码
				// 一次性合成超过300字符可考虑长文本语音合成功能.
				Request->setText(TCHAR_TO_UTF8(*InConent));

				// 发音人, 包含"xiaoyun", "ruoxi", "xiaogang"等. 可选参数, 默认是xiaoyun
				Request->setVoice("siqi");

				//Request->setEnableOnMessage(true);

				// 音量, 范围是0~100, 可选参数, 默认50
				Request->setVolume(50);

				// 音频编码格式, 可选参数, 默认是wav. 支持的格式pcm, wav, mp3
				Request->setFormat("wav");

				Request->setUrl("wss://nls-gateway-cn-shanghai.aliyuncs.com/ws/v1");

				// 设置音频数据采样率。可选参数，目前支持16000/8000。默认为16000。
				Request->setSampleRate(InCofigInfo.SampleRate);

				// 语速, 范围是-500~500, 可选参数, 默认是0
				Request->setSpeechRate(0);

				// 语调, 范围是-500~500, 可选参数, 默认是0
				Request->setPitchRate(0);

				// 开启字幕
				Request->setEnableSubtitle(true);

				// 访问个性化音色，访问的Voice必须是个人定制音色
				//例子：Request->setPayloadParam("{\"enable_ptts\":true}");
				if (!InCofigInfo.PayloadParam.IsEmpty())
				{
					Request->setPayloadParam(TCHAR_TO_UTF8(*InCofigInfo.PayloadParam));
				}

				//start()为异步操作。成功则开始返回BinaryRecv事件。失败返回TaskFailed事件。
				int32 ReturnValue = Request->start();
				if (ReturnValue < 0)
				{
					NLS_ERROR_PRINT(TEXT("start() failed. may be can not connect server. please check network or firewalld"));

					// start()失败，释放request对象。
					NlsClient::getInstance()->releaseSynthesizerRequest(Request);
					delete ParamCallBack;

					return;
				}
				else
				{
					//通知云端数据发送结束.
					// stop()为无意义接口，调用与否都会跑完全程，均需等待closed事件回调.
					// cancel()立即停止工作, 且不会有回调返回, 失败返回TaskFailed事件。
					ReturnValue = Request->stop();
					if (ReturnValue == 0)
					{
						NLS_LOG_PRINT(TEXT("wait closed callback."));

						// 语音服务器存在来不及处理当前请求, 10s内不返回任何回调的问题,
						// 然后在10s后返回一个TaskFailed回调, 错误信息为:
						// "Gateway:IDLE_TIMEOUT:Websocket session is idle for too long time, the last directive is 'StopRecognition'!"
						// 所以需要设置一个超时机制.
						ParamCallBack->Type = ENlsParamCallBackType::CALLBACK_TIMEOUT;

						ParamCallBack->Event->Wait(10000);
						if (ParamCallBack->Type == ENlsParamCallBackType::CALLBACK_TIMEOUT)
						{
							NLS_ERROR_PRINT(TEXT("Stop timeout."));
							Request->cancel();

							NlsClient::getInstance()->releaseSynthesizerRequest(Request);
							delete ParamCallBack;

							return;
						}
					}
					else
					{
						NLS_ERROR_PRINT(TEXT("stop ret is %d"), ReturnValue);
					}

					NlsClient::getInstance()->releaseSynthesizerRequest(Request);
					delete ParamCallBack;

					return;
				}
			}
			else
			{
				NLS_ERROR_PRINT(TEXT("createSynthesizerRequest fail"));
			}
		}
		else
		{
			SimpleAlibabaCloudVoiceHttp::FHTTPDelegate Delegate;
			Delegate.SimpleCompleteDelegate.BindRaw(this, &FSimpleAlibabaCloudVoiceManage::OnRequestComplete, ParamCallBack);
			Delegate.SimpleSingleRequestProgressDelegate.BindRaw(this, &FSimpleAlibabaCloudVoiceManage::OnRequestProgress, ParamCallBack);
			Delegate.SimpleSingleRequestHeaderReceivedDelegate.BindRaw(this, &FSimpleAlibabaCloudVoiceManage::OnRequestHeaderReceived, ParamCallBack);

			ParamCallBack->HTTP = SimpleAlibabaCloudVoiceHttp::FHTTP::CreateHTTPObject(Delegate);

			FSpeechSynthesizerParam Param;
			Param.Heard.Appkey = FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.AppKey;
			Param.Heard.Token = FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token;

			Param.Payload.TTSRequest.SampleRate = InCofigInfo.SampleRate;
			Param.Payload.TTSRequest.Text = InConent;
			//Param.Payload.TTSRequest.Voice

			FString JsonContent;
			SimpleAlibabaCluoudVoiceJson::SpeechSynthesizerParamToJsonString(Param, JsonContent);

			TMap<FString, FString> InCustomMetadataHeader;
			InCustomMetadataHeader.Add(TEXT("Content-Type"), TEXT("application/json"));
			InCustomMetadataHeader.Add(TEXT("SpeechSynthesizerRequestStage"),FSpeechSynthesizerRequestStage::RequestMergeStage);

			if (ParamCallBack->HTTP->Request(
				FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.URL,
				JsonContent, InCustomMetadataHeader))
			{
				NLS_LOG_PRINT(TEXT("Post Request OK."));
			}
			else
			{
				NLS_LOG_PRINT(TEXT("Post Request fail."));
			}
		}	
	}
}

bool FSimpleAlibabaCloudVoiceManage::GenerateToken(
	FString& OutToken, 
	uint32& OutExpireTime)
{
	AlibabaNlsCommon::NlsToken NlsTokenRequest;

	NlsTokenRequest.setAccessKeyId(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.ID));
	NlsTokenRequest.setKeySecret(TCHAR_TO_UTF8(*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Secret));

	int32 RetCode = NlsTokenRequest.applyNlsToken();
	if (RetCode < 0)
	{
		NLS_ERROR_PRINT(TEXT("Failed error code:%i"),
			RetCode,
			UTF8_TO_TCHAR(NlsTokenRequest.getErrorMsg()));

		return false;
	}

	OutToken = UTF8_TO_TCHAR(NlsTokenRequest.getToken());
	OutExpireTime = NlsTokenRequest.getExpireTime();

	return true;
}

bool FSimpleAlibabaCloudVoiceManage::IsExpire(uint32 InExpireTime) const
{
	std::time_t CurrentTime = std::time(0);

	return InExpireTime - CurrentTime < 10;
}

bool FSimpleAlibabaCloudVoiceManage::CheckTokenExpireAndGenerateNewToken()
{
	uint32 ExpiredTime = 0;
	if (FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token.IsEmpty())
	{
		if (GenerateToken(FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token,
			ExpiredTime))
		{
			FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.ExpireTime = ExpiredTime;
			FSimpleAlibabaCloudVoiceConfig::Get()->Save();

			NLS_LOG_PRINT(TEXT("the token will be expired, please generate new token by AccessKey-ID and AccessKey-Secret."));

			return true;
		}
	}
	else
	{
		if (IsExpire(FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.ExpireTime))
		{
			if (GenerateToken(FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token,
				ExpiredTime))
			{
				NLS_LOG_PRINT(TEXT("the token will be expired, please generate new token by AccessKey-ID and AccessKey-Secret."));
				
				FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.ExpireTime = ExpiredTime;
				FSimpleAlibabaCloudVoiceConfig::Get()->Save();

				return true;
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}

void FSimpleAlibabaCloudVoiceManage::OnRequestComplete(
	FHttpRequestPtr HttpRequest,
	FHttpResponsePtr HttpResponse,
	bool bSucceeded,
	FNlsParamCallBack* InCallbackParam)
{
	if (bSucceeded)
	{
		FSpeechSynthesizerResponded Responded;
		SimpleAlibabaCluoudVoiceJson::JsonStringToSpeechSynthesizerResponded(
			HttpResponse->GetContentAsString(),
			Responded);
	
		if (HttpResponse->GetResponseCode() == 200)
		{
			if (HttpRequest->GetHeader(TEXT("SpeechSynthesizerRequestStage")).Equals(FSpeechSynthesizerRequestStage::RequestMergeStage))
			{
				if (!Responded.Data.TaskId.IsEmpty())
				{
					//https://nls-gateway.cn-shanghai.aliyuncs.com/rest/v1/tts/async?appkey={Appkey}&task_id={task_id}&token={Token}
					OnRequestAudioAddress(Responded, InCallbackParam);
				}
			}
			else if (HttpRequest->GetHeader(TEXT("SpeechSynthesizerRequestStage")).Equals(FSpeechSynthesizerRequestStage::RequestAudioAddress))
			{
				NLS_LOG_PRINT(TEXT("GET AudioAddress=%s"), *Responded.Data.AudioAddress);

				//轮询
				if (Responded.ErrorMessage.Equals(TEXT("QUEUEING")) || //排队 可能有其他用户
					Responded.ErrorMessage.Equals(TEXT("RUNNING")))//正在合成你的任务
				{
					FPlatformProcess::Sleep(0.1f);

					OnRequestAudioAddress(Responded, InCallbackParam);
				}
				else if (Responded.ErrorMessage.Equals(TEXT("SUCCESS")))//合成完毕 可以GET
				{
					TMap<FString, FString> InCustomMetadataHeader;
					InCustomMetadataHeader.Add(TEXT("SpeechSynthesizerRequestStage"), FSpeechSynthesizerRequestStage::RequestDownloadAudio);
					if (InCallbackParam->HTTP->Request(Responded.Data.AudioAddress, TEXT(""),
						InCustomMetadataHeader,
						SimpleAlibabaCloudVoiceHttp::EHTTPVerbType::GET))
					{

					}
				}
				else
				{
					NLS_ERROR_PRINT(TEXT("POST ErrorMessage=%s,ErrorCode=%i,Status=%i"),
						*Responded.ErrorMessage,
						Responded.ErrorCode,
						Responded.Status);

					delete InCallbackParam;
				}
			}
			else if (HttpRequest->GetHeader(TEXT("SpeechSynthesizerRequestStage")).Equals(FSpeechSynthesizerRequestStage::RequestDownloadAudio))
			{
				if (HttpResponse->GetContent().Num() > 0)
				{
					if (!InCallbackParam->AudioFullFile.IsEmpty())
					{
						FFileHelper::SaveArrayToFile(
							HttpResponse->GetContent(),
							*InCallbackParam->AudioFullFile);
					}
					FNlsEvent UENlsEvent;
					if (InCallbackParam->bAutomaticallyPlaySound)
					{
						const uint8* SoundPtr = HttpResponse->GetContent().GetData();
						SimpleSoundHandle::SimplePlaySoundByMemory(SoundPtr);
					}
					else
					{
						UENlsEvent.Data = HttpResponse->GetContent();
					}
								
					UENlsEvent.MsgType = ENlsEventType::SynthesisCompleted;
					UENlsEvent.StatusCode = HttpResponse->GetResponseCode();
					
					CALL_PROGRESSCALLBACK_GAMETHREAD(UENlsEvent);
				}

				delete InCallbackParam;
			}
		}
		else
		{
			NLS_ERROR_PRINT(TEXT("POST ErrorMessage=%s,ErrorCode=%i,Status=%i"),
				*Responded.ErrorMessage,
				Responded.ErrorCode,
				Responded.Status);

			delete InCallbackParam;
		}
	}
}

void FSimpleAlibabaCloudVoiceManage::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 SendBytes, int32 BytesReceived, FNlsParamCallBack* InCallbackParam)
{

}

void FSimpleAlibabaCloudVoiceManage::OnRequestHeaderReceived(FHttpRequestPtr Request, const FString& HeaderName, const FString& NewHeaderValue, FNlsParamCallBack* ParamCallBack)
{

}

void FSimpleAlibabaCloudVoiceManage::OnRequestAudioAddress(const FSpeechSynthesizerResponded& Responded, FNlsParamCallBack* InCallbackParam)
{
	if (InCallbackParam)
	{
		FString URL = FString::Printf(TEXT("%s?appkey=%s&task_id=%s&token=%s"),
			*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.URL,
			*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.AppKey,
			*Responded.Data.TaskId,
			*FSimpleAlibabaCloudVoiceConfig::Get()->UserInfo.Token);

		TMap<FString, FString> InCustomMetadataHeader;
		InCustomMetadataHeader.Add(TEXT("SpeechSynthesizerRequestStage"), FSpeechSynthesizerRequestStage::RequestAudioAddress);

		if (InCallbackParam->HTTP->Request(URL, TEXT(""),
			InCustomMetadataHeader,
			SimpleAlibabaCloudVoiceHttp::EHTTPVerbType::GET))
		{

		}
	}	
}

int32 FSimpleAlibabaCloudVoiceManage::GetSpeechTranscriberHandle()
{
	FScopeLock ScopeLock(&Mutex);

	int32 InHandle = CreateUniqueHandle();
	SpeechTranscriberPool.Add(InHandle,false);

	return InHandle;
}

bool FSimpleAlibabaCloudVoiceManage::RemoveSpeechTranscriberHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&Mutex);

	return SpeechTranscriberPool.Remove(InHandle) != 0;
}

bool FSimpleAlibabaCloudVoiceManage::FindSpeechTranscriberHandle(int32 InHandle)
{
	FScopeLock ScopeLock(&Mutex);

	if (bool* InValue = SpeechTranscriberPool.Find(InHandle))
	{
		return *InValue;
	}

	return true;
}

int32 FSimpleAlibabaCloudVoiceManage::CreateUniqueHandle()
{
	int32 Index = FMath::RandRange(0, 999999999);

	if(SpeechTranscriberPool.Find(Index))
	{
		return CreateUniqueHandle();
	}

	return Index;
}

bool FSimpleAlibabaCloudVoiceManage::IsSpeechTranscriberHandleExit(int32 InHandle) const
{
	return SpeechTranscriberPool.Find(InHandle) != NULL;
}

#if PLATFORM_WINDOWS
#pragma optimize("",on) 
#endif