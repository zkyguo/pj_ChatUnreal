// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"

namespace SimpleAlibabaCluoudVoiceJson
{
	SIMPLEALIBABACLOUDVOICE_API void SpeechSynthesizerParamToJsonString(const FSpeechSynthesizerParam &InParam,FString &OutJsonString);
	SIMPLEALIBABACLOUDVOICE_API void JsonStringToSpeechSynthesizerResponded(const FString& JsonString, FSpeechSynthesizerResponded& OutResponded);
}