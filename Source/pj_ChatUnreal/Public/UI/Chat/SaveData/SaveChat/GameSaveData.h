#pragma once

#include "CoreMinimal.h"
#include "Core/ChatSaveCore.h"
#include "UI/Chat/SaveData/ChatSaveData.h"
#include "GameSaveData.generated.h"



UCLASS()
class UGameSaveData : public UChatSaveCore
{
	GENERATED_BODY()

	UPROPERTY(SaveGame)
	TArray<FChatSaveData> ChatDatas;



public:
	virtual void InitSaveChat(UWorld* InWorld) override;
	virtual void InitSaveChatFromArchive(UWorld* InWorld) override;


};
