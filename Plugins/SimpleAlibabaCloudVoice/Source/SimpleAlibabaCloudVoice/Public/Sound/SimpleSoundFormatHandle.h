// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SimpleAlibabaCloudVoiceType.h"

namespace SimpleSoundFormatHandle
{
	SIMPLEALIBABACLOUDVOICE_API int32 PCMToWAVByMemory(int32 InChannels,int32 InSampleRate,const TArray<uint8> &InPCMData,TArray<uint8>& OutWAVData);
}