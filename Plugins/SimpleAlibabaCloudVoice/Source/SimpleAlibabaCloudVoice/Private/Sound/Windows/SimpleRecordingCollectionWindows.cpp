// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "Sound/Windows/SimpleRecordingCollectionWindows.h"

#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)

FSimpleRecordingCollectionWindows::FSimpleRecordingCollectionWindows()
{
	Buffer = NULL;
	bSpeak = false;
	EvenHandle = NULL;
	BufferSize = 1024 * 10;
	ExtractDataTime = 100;

	WaveformatSetting.wFormatTag = WAVE_FORMAT_PCM;
	WaveformatSetting.nSamplesPerSec = 16000;
	WaveformatSetting.wBitsPerSample = 16;
	WaveformatSetting.nChannels = 1;

	WaveformatSetting.nAvgBytesPerSec =
		WaveformatSetting.nSamplesPerSec *
		WaveformatSetting.nChannels *
		WaveformatSetting.wBitsPerSample / 8;

	WaveformatSetting.nBlockAlign =
		WaveformatSetting.nChannels *
		WaveformatSetting.wBitsPerSample / 8;

	WaveformatSetting.cbSize = 0;

	Buffer = new uint8[BufferSize];
	FMemory::Memzero(Buffer,BufferSize);
}

void FSimpleRecordingCollectionWindows::InintConfig(const FRecordingConfig& InConfig)
{
	WaveformatSetting.wFormatTag = WAVE_FORMAT_PCM;
	WaveformatSetting.nSamplesPerSec = InConfig.SampleRate;
	WaveformatSetting.wBitsPerSample = InConfig.BitsPerSample;
	WaveformatSetting.nChannels = InConfig.Channels;
	ExtractDataTime = InConfig.ExtractDataTime;

	WaveformatSetting.nAvgBytesPerSec =
		WaveformatSetting.nSamplesPerSec *
		WaveformatSetting.nChannels *
		WaveformatSetting.wBitsPerSample / 8;

	WaveformatSetting.nBlockAlign =
		WaveformatSetting.nChannels *
		WaveformatSetting.wBitsPerSample / 8;

	WaveformatSetting.cbSize = 0;

	BufferSize = InConfig.BufferSize;

	if (Buffer)
	{
		delete Buffer;
		Buffer = NULL;
	}

	Buffer = new uint8[BufferSize];
	FMemory::Memzero(Buffer, BufferSize);
}

bool FSimpleRecordingCollectionWindows::StartSpeak()
{
	EvenHandle = CreateEvent(NULL, 0, 0, NULL);

	//CALLBACK_EVENT		dwCallback 参数是事件句柄。
	//CALLBACK_FUNCTION 	dwCallback 参数是回调过程地址。
	//CALLBACK_NULL			无回调机制。 这是默认设置。
	//CALLBACK_THREAD		dwCallback 参数是线程标识符。
	//CALLBACK_WINDOW		dwCallback 参数是窗口句柄。
	//WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE		
	//WAVE_FORMAT_DIRECT	ACM 驱动程序不会对音频数据执行转换。（Audio Compression Manager或Audio Codec Manager）
	//WAVE_FORMAT_QUERY
	//WAVE_MAPPED
	waveInOpen(&Wavein,WAVE_MAPPER,&WaveformatSetting, (DWORD_PTR)EvenHandle, 0L, CALLBACK_EVENT);
	//MMSYSERR_ALLOCATED	已分配指定资源。
	//MMSYSERR_NODRIVER		不存在设备驱动程序。
	//MMSYSERR_NOMEM		无法分配或锁定内存
	//WAVERR_BADFORMAT		尝试使用不受支持的波形音频格式打开。

	bSpeak = true;

	return true;
}

bool FSimpleRecordingCollectionWindows::StopSpeak()
{
	bSpeak = false;

	waveInClose(Wavein);

	return true;
}

uint8* FSimpleRecordingCollectionWindows::GetBuffer(int32& OutSize)
{
	ClearBuffer();

	Whdr.lpData = (LPSTR)Buffer;
	Whdr.dwBufferLength = BufferSize;
	Whdr.dwBytesRecorded = 0;
	Whdr.dwUser = 0;
	Whdr.dwFlags = 0;
	Whdr.dwLoops = 1;

	//装备波型数据
	waveInPrepareHeader(Wavein, &Whdr, sizeof(WAVEHDR));

	//指定波形为录音数据缓存
	waveInAddBuffer(Wavein, &Whdr, sizeof(WAVEHDR));

	//开始录音
	waveInStart(Wavein);

	//等待一段时间 注意 不要让主线程执行当前接口
	Sleep(ExtractDataTime);

	//停止录音
	waveInReset(Wavein);

	OutSize = Whdr.dwBytesRecorded;

	return Buffer;
}

void FSimpleRecordingCollectionWindows::ClearBuffer()
{
	FMemory::Memzero(Buffer, BufferSize);
}

bool FSimpleRecordingCollectionWindows::IsSpeak()const
{
	return bSpeak;
}
#endif