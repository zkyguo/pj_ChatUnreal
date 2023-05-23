#pragma once

#include "CoreMinimal.h"
#include <UObject/ObjectMacros.h>
#include "ChatSaveData.generated.h"


USTRUCT()
struct FChatSaveData
{
	GENERATED_BODY()

	FChatSaveData();

	UPROPERTY(SaveGame)
	int32 ID;

	UPROPERTY(SaveGame)
	FString InContent;

	UPROPERTY(SaveGame)
	FString Time;
};

