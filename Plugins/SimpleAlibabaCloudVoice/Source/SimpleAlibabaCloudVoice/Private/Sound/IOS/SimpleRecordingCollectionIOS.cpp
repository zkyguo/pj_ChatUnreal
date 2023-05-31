// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "Sound/IOS/SimpleRecordingCollectionIOS.h"

#if defined(__APPLE__) || defined(TARGET_IPHONE_SIMULATOR) ||defined( TARGET_OS_IPHONE) || defined(TARGET_OS_IPHONE)

void FSimpleRecordingCollectionIOS::InintConfig(const FRecordingConfig& InConfig)
{

}

bool FSimpleRecordingCollectionIOS::StartSpeak()
{
	return false;
}

bool FSimpleRecordingCollectionIOS::StopSpeak()
{
	return false;
}

uint8* FSimpleRecordingCollectionIOS::GetBuffer(int32& OutSize)
{
	return NULL;
}

void FSimpleRecordingCollectionIOS::ClearBuffer()
{

}

bool FSimpleRecordingCollectionIOS::IsSpeak()const
{
	return false;
}
#endif // #if __APPLE__ || TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE || TARGET_OS_IPHONE
