// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"

namespace SimpleSoundHandle
{
	SIMPLEALIBABACLOUDVOICE_API bool SimplePlaySoundByFile(const FString &InFilePath);
	SIMPLEALIBABACLOUDVOICE_API bool SimplePlaySoundByMemory(const uint8* SoundPtr);
	SIMPLEALIBABACLOUDVOICE_API bool SimpleStopSound();
}