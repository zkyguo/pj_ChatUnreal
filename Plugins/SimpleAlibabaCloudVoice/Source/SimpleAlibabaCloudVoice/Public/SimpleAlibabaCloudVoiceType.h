// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "HTTP/SimpleAlibabaCloudVoiceHttp.h"
#include "SimpleAlibabaCloudVoiceType.generated.h"

UENUM(BlueprintType)
enum class ENlsParamCallBackType :uint8
{
	CALLBACK_TIMEOUT,
	CALLBACK_SUCCESS,
	CALLBACK_CHANNEL_CLOSED_SUCCESS,
	CALLBACK_FAIL,
};

UENUM(BlueprintType)
enum class ENlsAudioEncodingFormat :uint8
{
	PCM,//
	OPUS,
};

UENUM(BlueprintType)
enum class ENlsEventType :uint8
{
	TaskFailed = 0,
	RecognitionStarted,
	RecognitionCompleted,
	RecognitionResultChanged,
	WakeWordVerificationCompleted,
	TranscriptionStarted,
	SentenceBegin,
	TranscriptionResultChanged,
	SentenceEnd,
	SentenceSemantics,
	TranscriptionCompleted = 10,
	SynthesisStarted,
	SynthesisCompleted,
	Binary,
	MetaInfo,
	DialogResultGenerated,
	Close = 16,  /*语音功能通道连接关闭*/
	Message
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FNlsLogConfig
{
	GENERATED_USTRUCT_BODY()

	FNlsLogConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|LogConfig")
	FString NlsPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|LogConfig")
	int32 LogFileSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|LogConfig")
	int32 LogFileNum;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FNlsUserInfo
{
	GENERATED_USTRUCT_BODY()

	FNlsUserInfo();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|UserInfo")
	FString Token;//不要序列化到本地

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|UserInfo")
	FString AppKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|UserInfo")
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|UserInfo")
	FString Secret;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|UserInfo")
	int32 ExpireTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|URL")
	FString URL;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FNlsInfo
{
	GENERATED_USTRUCT_BODY()

	FNlsInfo();

	// 设置音频数据编码格式。可选参数，目前支持PCM/OPUS，默认为PCM
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	ENlsAudioEncodingFormat EncodingFormat;

	// 设置音频数据采样率。可选参数，目前支持16000/8000。默认为16000。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	int32 SampleRate;

	// 设置是否返回中间识别结果。可选参数，默认false。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	bool bIntermediateResult;

	// 设置是否在后处理中添加标点。可选参数，默认false。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	bool bPunctuationPrediction;

	// 设置是否在后处理中执行ITN。可选参数，默认false。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	bool bInverseTextNormalization;

	// 语音断句检测阈值，一句话之后静音长度超过该值，即本句结束，合法参数范围200ms～2000ms，默认值800ms。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo|Transcriber")
	int32 MaxSentenceSilence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo|Recognizer")
	bool bEnableVoiceDetection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo|Recognizer", meta = (EditCondition = "bEnableVoiceDetection"))
	int32 MaxStartSilence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo|Recognizer", meta = (EditCondition = "bEnableVoiceDetection"))
	int32 MaxEndSilence;

	//定制语言模型id，可选。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	FString CustomizationId;
	
	// 定制泛热词id，可选。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	FString VocabularyId;

	//用于传递某些定制化、高级参数设置，参数格式为JSON格式：{ "key": "value" }。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	FString PayloadParam;

	// UTF-8 or GBK
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|NlsInfo")
	FString OutputFormat;

	//是否自动播放下载的数据 如果true data数据为空 false 需要手动播放
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Synthesizer")
	bool bAutomaticallyPlaySound;

public:
	FString GetEncodingFormatString() const;
	ENlsAudioEncodingFormat GetNlsAudioEncodingFormatByString(const FString &InString);
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FNlsEvent
{
	GENERATED_USTRUCT_BODY()

	FNlsEvent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	int32 StatusCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	FString Msg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	ENlsEventType MsgType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	FString TaskId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	FString Result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	FString DisplayText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	FString SpokenText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	int32 SentenceTimeOutStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	int32 SentenceIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	int32 SentenceTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	int32 SentenceBeginTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	double SentenceConfidence;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|Event")
	TArray<uint8> Data;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FNlsParamCallBack
{
	GENERATED_USTRUCT_BODY()

	FNlsParamCallBack();
	~FNlsParamCallBack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack")
	ENlsParamCallBackType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack")
	FGuid ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack")
	FNlsInfo Info;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack")
	bool bEnableGlobalConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack|Synthesizer")
	TArray<uint8> BinaryData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack|Synthesizer")
	FString AudioFullFile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack|Synthesizer")
	bool bAutomaticallyPlaySound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Nls|ParamCallBack|Transcriber")
	int32 Handle;

	FEvent* Event;

	TSharedPtr<SimpleAlibabaCloudVoiceHttp::FHTTP> HTTP;

	TFunction<void(const FNlsEvent&)> ProgressCallback;
};

//tts请求
USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerTTSRequest
{
	GENERATED_USTRUCT_BODY()

	FSpeechSynthesizerTTSRequest();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|TTSRequestc|SpeechSynthesizer")
	FString Voice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|TTSRequestc|SpeechSynthesizer")
	int32 SampleRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|TTSRequestc|SpeechSynthesizer")
	FString Format;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|TTSRequestc|SpeechSynthesizer")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|TTSRequestc|SpeechSynthesizer")
	bool bEnableSubtitle;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerContext
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Context|SpeechSynthesizer")
	FString DeviceID;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerPayload
{
	GENERATED_USTRUCT_BODY()

	FSpeechSynthesizerPayload();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Payload|SpeechSynthesizer")
	FSpeechSynthesizerTTSRequest TTSRequest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Payload|SpeechSynthesizer")
	bool bEnableNotify;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerHeard
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Heard|SpeechSynthesizer")
	FString Appkey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Heard|SpeechSynthesizer")
	FString Token;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerParam
{
	GENERATED_USTRUCT_BODY()

	FSpeechSynthesizerParam(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Param|SpeechSynthesizer")
	FSpeechSynthesizerPayload Payload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Param|SpeechSynthesizer")
	FSpeechSynthesizerContext Context;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Param|SpeechSynthesizer")
	FSpeechSynthesizerHeard Heard;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerSentences
{
	GENERATED_USTRUCT_BODY()

	FSpeechSynthesizerSentences();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Sentences|SpeechSynthesizer")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Sentences|SpeechSynthesizer")
	int32 BeginTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Sentences|SpeechSynthesizer")
	int32 EndTime;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerDataResponded
{
	GENERATED_USTRUCT_BODY()

	FSpeechSynthesizerDataResponded(){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|DataResponded|SpeechSynthesizer")
	TArray<FSpeechSynthesizerSentences> Sentences;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|DataResponded|SpeechSynthesizer")
	FString TaskId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|DataResponded|SpeechSynthesizer")
	FString AudioAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|DataResponded|SpeechSynthesizer")
	FString NotifyCustom;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FSpeechSynthesizerResponded
{
	GENERATED_USTRUCT_BODY()

	FSpeechSynthesizerResponded();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Responded|SpeechSynthesizer")
	int32 Status;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Responded|SpeechSynthesizer")
	int32 ErrorCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Responded|SpeechSynthesizer")
	FString ErrorMessage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Responded|SpeechSynthesizer")
	FString RequestID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Responded|SpeechSynthesizer")
	FString URL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HTTP|Responded|SpeechSynthesizer")
	FSpeechSynthesizerDataResponded Data;
};

USTRUCT(BlueprintType)
struct SIMPLEALIBABACLOUDVOICE_API FRecordingConfig
{
	GENERATED_USTRUCT_BODY()

	FRecordingConfig();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecordingConfig")
	int32 SampleRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecordingConfig")
	int32 Channels;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecordingConfig")
	int32 BitsPerSample;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecordingConfig")
	int32 BufferSize;

	//单位是毫秒
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RecordingConfig")
	int32 ExtractDataTime;
};

struct SIMPLEALIBABACLOUDVOICE_API FWaveHeard
{
	int8 ChunkID[4];
	uint32 ChunkSize;
	int8 Format[4];
};

struct SIMPLEALIBABACLOUDVOICE_API FWaveFmt
{
	int8 SubChunkID[4];
	uint32 SubChunkSize;
	uint16 AudioFormat;
	uint16 NumChannel;
	uint32 SampleRate;
	uint32 ByteRate;
	uint16 BlockAlign;
	uint16 BitsForSample;
};

struct SIMPLEALIBABACLOUDVOICE_API FWaveData
{
	int8 DataChunkID[4];
	uint32 DataChunkSize;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FNlsCallbackDelegate, const FNlsEvent&, NlsMyEvent);