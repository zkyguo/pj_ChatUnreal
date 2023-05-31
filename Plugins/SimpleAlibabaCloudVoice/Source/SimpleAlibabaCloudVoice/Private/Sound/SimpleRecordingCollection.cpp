// Copyright (C) RenZhai.2023.All Rights Reserved.

#include "Sound/SimpleRecordingCollection.h"
#include "Sound/Windows/SimpleRecordingCollectionWindows.h"
#include "Sound/IOS/SimpleRecordingCollectionIOS.h"

class FSimpleRecordingCollectionNone :public FSimpleRecordingCollection
{
public:
	virtual void InintCofig(const FRecordingConfig& InConfig){}
	virtual bool StartSpeak() { return false; }
	virtual bool StopSpeak() { return false; }
	virtual uint8* GetBuffer(int32& OutSize) { return NULL; }
	virtual void ClearBuffer(){}
	virtual bool IsSpeak()const { return false; }
};

TSharedPtr<FSimpleRecordingCollection> FSimpleRecordingCollection::CreateRecordingCollection()
{
#if defined(__WINDOWS__) || defined(_WIN32) || defined(WIN32) || defined(_WIN64) || defined(WIN64) || defined(__WIN32__) || defined(__TOS_WIN__)
	return MakeShareable<FSimpleRecordingCollectionWindows>(new FSimpleRecordingCollectionWindows());
#elif defined(__APPLE__) || defined(TARGET_IPHONE_SIMULATOR) ||defined( TARGET_OS_IPHONE) || defined(TARGET_OS_IPHONE)
	return MakeShareable<FSimpleRecordingCollectionIOS>(new FSimpleRecordingCollectionIOS());
#else
	return MakeShareable<FSimpleRecordingCollection>(new FSimpleRecordingCollectionNone());
#endif
}