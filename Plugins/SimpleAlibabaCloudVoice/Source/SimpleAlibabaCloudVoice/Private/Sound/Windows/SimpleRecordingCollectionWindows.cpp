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

	//CALLBACK_EVENT		dwCallback �������¼������
	//CALLBACK_FUNCTION 	dwCallback �����ǻص����̵�ַ��
	//CALLBACK_NULL			�޻ص����ơ� ����Ĭ�����á�
	//CALLBACK_THREAD		dwCallback �������̱߳�ʶ����
	//CALLBACK_WINDOW		dwCallback �����Ǵ��ھ����
	//WAVE_MAPPED_DEFAULT_COMMUNICATION_DEVICE		
	//WAVE_FORMAT_DIRECT	ACM �������򲻻����Ƶ����ִ��ת������Audio Compression Manager��Audio Codec Manager��
	//WAVE_FORMAT_QUERY
	//WAVE_MAPPED
	waveInOpen(&Wavein,WAVE_MAPPER,&WaveformatSetting, (DWORD_PTR)EvenHandle, 0L, CALLBACK_EVENT);
	//MMSYSERR_ALLOCATED	�ѷ���ָ����Դ��
	//MMSYSERR_NODRIVER		�������豸��������
	//MMSYSERR_NOMEM		�޷�����������ڴ�
	//WAVERR_BADFORMAT		����ʹ�ò���֧�ֵĲ�����Ƶ��ʽ�򿪡�

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

	//װ����������
	waveInPrepareHeader(Wavein, &Whdr, sizeof(WAVEHDR));

	//ָ������Ϊ¼�����ݻ���
	waveInAddBuffer(Wavein, &Whdr, sizeof(WAVEHDR));

	//��ʼ¼��
	waveInStart(Wavein);

	//�ȴ�һ��ʱ�� ע�� ��Ҫ�����߳�ִ�е�ǰ�ӿ�
	Sleep(ExtractDataTime);

	//ֹͣ¼��
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