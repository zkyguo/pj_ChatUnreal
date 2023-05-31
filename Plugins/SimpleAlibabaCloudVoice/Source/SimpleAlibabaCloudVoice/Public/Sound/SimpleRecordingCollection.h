// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"

class SIMPLEALIBABACLOUDVOICE_API FSimpleRecordingCollection :public TSharedFromThis<FSimpleRecordingCollection>
{
public:
	static TSharedPtr<FSimpleRecordingCollection> CreateRecordingCollection();

public:
	virtual void InintConfig(const FRecordingConfig &InConfig) = 0;

	virtual bool StartSpeak() = 0;

	virtual bool StopSpeak() = 0;

	virtual uint8 *GetBuffer(int32 &OutSize) = 0;

	virtual void ClearBuffer() = 0;

	virtual bool IsSpeak() const = 0;
};