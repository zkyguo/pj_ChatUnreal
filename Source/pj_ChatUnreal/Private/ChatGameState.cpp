// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatGameState.h"
#include "Kismet/GameplayStatics.h"

AChatGameState::AChatGameState()
{
	/// <summary>
/// If local save data exists, load data and get the recent save data
/// </summary>
	if (UChatSaveSlotList* LocalSaveData = Cast<UChatSaveSlotList>(UGameplayStatics::LoadGameFromSlot(TEXT("SlotList"), 0)))
	{
		SaveSlotList = LocalSaveData;
		FDateTime TmpSaveDateTime;
		for (auto& tmp : SaveSlotList->Slots)
		{
			FDateTime SaveDateTime;
			FDateTime::Parse(tmp.DateText, SaveDateTime);
			if (TmpSaveDateTime < SaveDateTime)
			{
				TmpSaveDateTime = SaveDateTime;
				currentSlotName = tmp.SlotName.ToString();
			}
		}

		LoadGameData(currentSlotName);
	}
	else //if not, create one
	{
		SaveSlotList = Cast<UChatSaveSlotList>(UGameplayStatics::CreateSaveGameObject(UChatSaveSlotList::StaticClass()));
		if (SaveSlotList)
		{
			FSaveSlot& InputSlot = SaveSlotList->Slots.AddDefaulted_GetRef();

			currentSlotName = TEXT("New Slot");

			InputSlot.DateText = FDateTime::Now().ToString();
			InputSlot.SlotName = FText::FromString(currentSlotName);

			if (UGameSaveData* NewSaveData = Cast<UGameSaveData>(UGameplayStatics::CreateSaveGameObject(UGameSaveData::StaticClass())))
			{
				SaveData.Add(InputSlot.SlotName.ToString(), NewSaveData);
				SaveGameData(InputSlot.SlotName.ToString());
			}
		}
	}
}

UGameSaveData* AChatGameState::FindCurrentGameSaveData()
{
	return NULL;
}

void AChatGameState::BeginPlay()
{
	Super::BeginPlay();


}

UGameSaveData* AChatGameState::FindSaveData(const FString& InName)
{
	if(const TObjectPtr<UGameSaveData> *InSaveData = SaveData.Find(InName))
	{
		return InSaveData->Get();
	}
	else
	{
		if(LoadGameData(InName))
		{
			if (const TObjectPtr<UGameSaveData>* SaveDataLoad = SaveData.Find(InName))
			{
				return SaveDataLoad->Get();
			}
		}
		
	}

	return NULL;
}

UChatSaveSlotList* AChatGameState::GetSaveSlotList()
{
	if(!SaveSlotList)
	{
		InitSaveData();
	}

	return SaveSlotList;
}

void AChatGameState::AddText(int32 InID, const FString& InContent)
{
	if (UGameSaveData* InSaveData = FindSaveData(currentSlotName))
	{
		FChatSaveData& InData = InSaveData->ChatDatas.AddDefaulted_GetRef();
		InData.ID = InID;
		InData.InContent = InContent;
		InData.Time = FDateTime::Now().ToString();

		SaveGameData(currentSlotName);
	}
}

void AChatGameState::InitSaveData()
{
}

bool AChatGameState::SaveGameData(const FString& InSlotName)
{
	if(SaveSlotList && !SaveData.IsEmpty())
	{
		if(UGameSaveData* InSaveData = FindSaveData(InSlotName))
		{
			return UGameplayStatics::SaveGameToSlot(SaveSlotList, FString::Printf(TEXT("SlotList")), 0) && 
				UGameplayStatics::SaveGameToSlot(InSaveData, InSlotName,0);
		}
	}

	return false;
}

bool AChatGameState::LoadGameData(const FString& InData)
{
	if(SaveData.Contains(InData))
	{
		if (UGameSaveData* InSaveData = Cast<UGameSaveData>(UGameplayStatics::LoadGameFromSlot(InData, 0)))
		{
			SaveData.Add(InData,InSaveData);
			return true;
		}
	}
	return false;
}

bool AChatGameState::DeleteGameData(const FString& SlotName)
{
	if (SaveData.Contains(SlotName) && UGameplayStatics::DeleteGameInSlot(SlotName,0))
	{
		FSaveSlot Slot;
		Slot.SlotName = FText::FromString(SlotName);

		SaveSlotList->Slots.Remove(Slot);

		return UGameplayStatics::SaveGameToSlot(SaveSlotList, FString::Printf(TEXT("SlotList")),0);
		
	}
	return false;
}


