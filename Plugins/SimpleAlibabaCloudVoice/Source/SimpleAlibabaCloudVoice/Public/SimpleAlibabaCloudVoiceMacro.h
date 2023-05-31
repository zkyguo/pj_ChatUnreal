// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#define NLS_PRINT(Content,CategoryName,...)\
{FString A_Msg = FString::Printf(Content,__VA_ARGS__); \
if (IsInGameThread())\
{\
	UE_LOG(SimpleAlibabaCloudVoiceLog, CategoryName, TEXT("%s"),*A_Msg); \
}\
else\
{\
	FFunctionGraphTask::CreateAndDispatchWhenReady([A_Msg]()\
	{\
		UE_LOG(SimpleAlibabaCloudVoiceLog, CategoryName,TEXT("%s"),*A_Msg);\
	}, TStatId(), NULL, ENamedThreads::GameThread);\
}}

#define NLS_LOG_PRINT(Content,...)\
NLS_PRINT(Content,Log,__VA_ARGS__)

#define NLS_ERROR_PRINT(Content,...)\
NLS_PRINT(Content,Error,__VA_ARGS__)

#define NLS_WARNING_PRINT(Content,...)\
NLS_PRINT(Content,Warning,__VA_ARGS__)

//3200字节
#define FRAME_SIZE 3200 

#define DEFINE_UE_NLSEVENT(EventName)\
FNlsEvent EventName;\
{\
	EventName.MsgType = (ENlsEventType)InEvent->getMsgType();\
	EventName.DisplayText = UTF8_TO_TCHAR(InEvent->getDisplayText());\
	EventName.StatusCode = InEvent->getStatusCode();\
	EventName.SentenceTimeOutStatus = InEvent->getSentenceTimeOutStatus();\
	EventName.SentenceIndex = InEvent->getSentenceIndex();\
	EventName.SentenceBeginTime = InEvent->getSentenceBeginTime();\
	EventName.SentenceConfidence = InEvent->getSentenceConfidence();\
	EventName.TaskId = UTF8_TO_TCHAR(InEvent->getTaskId());\
	EventName.Result = UTF8_TO_TCHAR(InEvent->getResult());\
	EventName.DisplayText = UTF8_TO_TCHAR(InEvent->getDisplayText());\
	EventName.SpokenText = UTF8_TO_TCHAR(InEvent->getSpokenText());\
}

#define CALL_PROGRESSCALLBACK_GAMETHREAD(EventName)\
if (IsInGameThread())\
{\
	InCallbackParam->ProgressCallback(EventName);\
}\
else\
{\
	auto TmpProgressCallback = InCallbackParam->ProgressCallback;\
	FFunctionGraphTask::CreateAndDispatchWhenReady([TmpProgressCallback,EventName]()\
	{\
		TmpProgressCallback(EventName);\
	}, TStatId(), NULL, ENamedThreads::GameThread); \
}
