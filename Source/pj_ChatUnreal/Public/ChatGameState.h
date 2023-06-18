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

public :
	UGameSaveData* FindCurrentGameSaveData();
	UGameSaveData* FindSaveData(const FString &InName);
	UChatSaveSlotList* GetSaveSlotList();

	void AddText(int32 InID,const FString &InData);
	void AddTexture2D(int32 InID, const TArray<UTexture2D*> &InTexture2D);

	void InitSaveData();

	bool SaveGameData(const FString& SlotName);
	UGameSaveData* LoadGameData(const FString& SlotName);

	FString AddGameData();
	bool DeleteGameData(const FString& SlotName);
	bool DeleteCurrentGameData();

	bool IsLastSavaDataSlot() const;
	FString GetUniqueSaveDataName();
	FString GetUniqueSaveDataName(const FString &InName, int32 index) const;

	void UpdateCurrentSlotName();

	void SetCurrentSlotName(const FString& Name);
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
