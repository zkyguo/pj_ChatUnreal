// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
#include "Sound/SimpleRecordingCollection.h"
#include "SimpleAlibabaCloudVoicePlatform.h"
#include <windows.h>
#include <mmsystem.h>
#include <mmreg.h>
#include <msacm.h>

class FSimpleRecordingCollectionWindows :public FSimpleRecordingCollection
{
public:
	FSimpleRecordingCollectionWindows();

	virtual void InintConfig(const FRecordingConfig& InConfig);

	virtual bool StartSpeak();

	virtual bool StopSpeak();

	virtual uint8* GetBuffer(int32& OutSize);

	virtual void ClearBuffer();

	virtual bool IsSpeak()const;

protected:
	WAVEFORMATEX WaveformatSetting;//采集音频格式
	HANDLE EvenHandle;
	HWAVEIN Wavein;//指向接收识别打开波形音频输入设备的句柄的缓冲区的指针
	bool bSpeak;
	uint8* Buffer;//接受使用
	int32 BufferSize;
	WAVEHDR Whdr;

	int32 ExtractDataTime;//录制的小段时间
};
#endif