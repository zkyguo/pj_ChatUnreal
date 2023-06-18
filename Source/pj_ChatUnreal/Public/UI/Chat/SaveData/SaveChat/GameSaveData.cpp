#include "GameSaveData.h"

#include "SimpleChatGPTLibrary.h"

void UGameSaveData::InitSaveChat(UWorld* InWorld)
{
	Super::InitSaveChat(InWorld);
	for(auto &tmp : ChatDatas)
	{
		if(tmp.TexturesPaths.Num()>0)
		{
			tmp.Textures.Empty();
			for (auto& path : tmp.TexturesPaths)
			{
				if (UTexture2D* texture = USimpleChatGPTLibrary::LocalImageToTexture2D(path))
				{
					tmp.Textures.Add(texture);
				}
			}
		}
	}
}

void UGameSaveData::InitSaveChatFromArchive(UWorld* InWorld)
{
	Super::InitSaveChatFromArchive(InWorld);
}
