#pragma once

#include "CoreMinimal.h"
#include "Core/ChatSaveCore.h"
#include <UI/Chat/SaveData/SaveSlot.h>
#include "ChatSaveSlotList.generated.h"




UCLASS()
class UChatSaveSlotList : public UChatSaveCore
{
	GENERATED_BODY()

public:
	virtual void InitSaveChat(UWorld* InWorld) override;
	virtual void InitSaveChatFromArchive(UWorld* InWorld) override;

	UPROPERTY(SaveGame)
	TMap<int32, FSaveSlot> Slots;
};
