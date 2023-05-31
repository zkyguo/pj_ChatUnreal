// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "Sound/SimpleSoundHandle.h"
#include "SimpleAlibabaCloudVoicePlatform.h"

namespace SimpleSoundHandle
{
	bool SimplePlaySoundByFile(const FString& InFilePath)
	{
		FString FullPath = FPaths::ConvertRelativePathToFull(InFilePath);

#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
		//SND_APPLICATION ��Ӧ�ó���ָ���Ĺ���������������
		//SND_ASYNC �첽���ţ����ű�������
		//SND_LOOP ѭ������
		//SND_NODEFAULT ����������Ϊʧ��
		//SND_FILENAME //�ļ���
		//SND_MEMORY //�ڴ������
		//SND_NOSTOP PlaySound�����ԭ����������������������FALSE��
		//SND_NOWAIT �������������æ�����Ͳ������������������ء�
		//SND_PURGE ֹͣ��������������йص�������������pszSoundΪNULL����ֹͣ���е�����������ֹͣpszSoundָ����������
		//SND_RESOURCE  pszSound������WAVE��Դ�ı�ʶ������ʱҪ�õ�hmod������
		return PlaySound(*FullPath, NULL, SND_FILENAME | SND_ASYNC);
#endif
		return false;
	}

	bool SimplePlaySoundByMemory(const uint8* SoundPtr)
	{
#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
		return PlaySound((LPCWSTR)SoundPtr, NULL, SND_MEMORY | SND_ASYNC);
#endif
		return false;
	}

	bool SimpleStopSound()
	{
#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
		return PlaySound(NULL, NULL, 0);
#endif
	}
}