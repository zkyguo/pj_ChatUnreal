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
	FString Name;

	UPROPERTY(SaveGame)
	FString Time;

	UPROPERTY()
	TArray<UTexture2D*> Textures;

	UPROPERTY()
	TArray<FString> TexturesPaths;
};

