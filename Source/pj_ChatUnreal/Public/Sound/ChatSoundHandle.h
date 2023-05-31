#pragma once

#include "CoreMinimal.h"

namespace ChatSoundHandle
{
	enum class ESoundHandleType : uint8
	{
		UE_Sound,
		AlibabaSound,
	};

	bool SpeakFromText(ESoundHandleType Intype, const FString& InTeller, const FString& InContent);
}
