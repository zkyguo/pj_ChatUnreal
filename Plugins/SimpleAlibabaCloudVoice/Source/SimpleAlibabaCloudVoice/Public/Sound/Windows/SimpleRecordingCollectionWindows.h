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
	WAVEFORMATEX WaveformatSetting;//�ɼ���Ƶ��ʽ
	HANDLE EvenHandle;
	HWAVEIN Wavein;//ָ�����ʶ��򿪲�����Ƶ�����豸�ľ���Ļ�������ָ��
	bool bSpeak;
	uint8* Buffer;//����ʹ��
	int32 BufferSize;
	WAVEHDR Whdr;

	int32 ExtractDataTime;//¼�Ƶ�С��ʱ��
};
#endif