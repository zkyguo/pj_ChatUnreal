// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"
#include "SimpleAlibabaCloudVoiceMacro.h"

class SIMPLEALIBABACLOUDVOICE_API FSimpleAlibabaCloudVoiceConfig
{
public:
	FSimpleAlibabaCloudVoiceConfig();
	~FSimpleAlibabaCloudVoiceConfig();

	static FSimpleAlibabaCloudVoiceConfig* Get();
	static void Destroy();

public:
	void Init(const FString& InPath = FPaths::ProjectConfigDir() / TEXT("NlsConfig.ini"));
	void Save(const FString& InPath = FPaths::ProjectConfigDir() / TEXT("NlsConfig.ini"));

public:
	FNlsLogConfig LogConfig;
	FNlsUserInfo UserInfo;
	FNlsInfo Info;//全局

private:
	static FSimpleAlibabaCloudVoiceConfig* Manage;
};
