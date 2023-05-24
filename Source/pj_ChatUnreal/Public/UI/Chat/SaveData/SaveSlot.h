#pragma once

#include "CoreMinimal.h"

#include "SaveSlot.generated.h"

USTRUCT()
struct FSaveSlot
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	FString DateText;

	UPROPERTY(SaveGame)
	FText SlotName;
};
