// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include <UI/Chat/SaveData/SaveChat/GameSaveData.h>
#include <UI/Chat/SaveData/SaveChat/ChatSaveSlotList.h>
#include "ChatGameState.generated.h"



/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API AChatGameState : public AGameStateBase
{
	GENERATED_BODY()

	AChatGameState();

	UGameSaveData* FindCurrentGameSaveData();
	UGameSaveData* FindSaveData(const FString &InName);
	UChatSaveSlotList* GetSaveSlotList() const;

	void AddText(int32 InID,const FString &InData);

	bool SaveGameData(const FString& InData);
	bool LoadGameData(const FString& InData);
	bool DeleteGameData(const FString& InData);

	void SetCurrentSlotName(const FString &Name) { currentSlotName = Name; }
	FString GetCurrentSlotName() const { return  currentSlotName; }
protected:
	virtual void BeginPlay() override;

private :
	UPROPERTY()
	TMap<FString, TObjectPtr<UGameSaveData>> SaveData;

	UPROPERTY()
	TObjectPtr<UChatSaveSlotList> SaveSlotList;

	UPROPERTY()
	FString currentSlotName;
};
