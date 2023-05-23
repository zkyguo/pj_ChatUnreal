#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ChatSaveCore.generated.h"

UCLASS()
class UChatSaveCore : public USaveGame
{
	GENERATED_BODY()

public:
	virtual void InitSaveChat(UWorld* InWorld); //Save
	virtual void InitSaveChatFromArchive(UWorld* InWorld); //loading
};
