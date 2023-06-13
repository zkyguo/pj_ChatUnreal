// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimpleChatGPTLibrary.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "ImageUtils.h"
#include "SimpleChatGPT.h"
#include "Engine/Texture2D.h"
#include "Misc/Base64.h"

USimpleChatGPTLibrary::USimpleChatGPTLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


UTexture2D* USimpleChatGPTLibrary::Base64ToTexture2D(const FString& InImageBase64String)
{
	TArray<uint8> ImageBytesData;
	if (FBase64::Decode(InImageBase64String, ImageBytesData))
	{
		if(TSharedPtr<IImageWrapper> PNGImageWrapper = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper").CreateImageWrapper(EImageFormat::PNG))
		{
			if(PNGImageWrapper->SetCompressed(ImageBytesData.GetData(), ImageBytesData.Max()))
			{
				TArray<uint8> UncompressedBGRA;
				if(PNGImageWrapper->GetRaw(ERGBFormat::BGRA,8,UncompressedBGRA))
				{
					if(UTexture2D* textureObject = UTexture2D::CreateTransient(
					PNGImageWrapper->GetWidth(),
					PNGImageWrapper->GetHeight()))
					{
						void* TexturePtr = textureObject->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(TexturePtr, UncompressedBGRA.GetData(), UncompressedBGRA.Num());
						textureObject->GetPlatformData()->Mips[0].BulkData.Unlock();

						textureObject->UpdateResource();
						return textureObject;
					}
				}
			}
		}
	}

	return NULL;
}

bool USimpleChatGPTLibrary::SaveTexture2DToLocalDisk(UTexture2D* Texture2D, const FString& InFileName)
{
	if(Texture2D)
	{
		FTexture2DMipMap Texture2DMipMap = Texture2D->GetPlatformData()->Mips[0];
		uint8* TexturePtr = const_cast<uint8*>(static_cast<const uint8*>(Texture2DMipMap.BulkData.LockReadOnly()));
		TArray<uint8> UncompressedBGRA;

		UncompressedBGRA.Append(TexturePtr, Texture2DMipMap.BulkData.GetBulkDataSize());
		Texture2DMipMap.BulkData.Unlock();

		TArray<uint8> CompressedBytes;
		if(CompressImageArray(Texture2D->GetSizeX(),Texture2D->GetSizeY(),UncompressedBGRA, CompressedBytes))
		{
			return FFileHelper::SaveArrayToFile(CompressedBytes, *InFileName);
		}
		
	}

	return false;
}

bool USimpleChatGPTLibrary::CompressImageArray(int32 sizeX, int32 sizeY,const TArray<uint8>& InUncompressedBGRA,
	TArray<uint8>& OutCompressedBytes)
{
	TArray<FColor> UncompressedColor;
	UncompressedColor.SetNum(OutCompressedBytes.Num() / 4);

	for(int32 i = 0; i < InUncompressedBGRA.Num(); i+=4)
	{
		int32 Index = i / 4;

		UncompressedColor[Index].B = InUncompressedBGRA[i + 2];
		UncompressedColor[Index].G = InUncompressedBGRA[i + 1];
		UncompressedColor[Index].R = InUncompressedBGRA[i];
		UncompressedColor[Index].A = InUncompressedBGRA[i + 3];
	}

	FImageUtils::CompressImageArray(sizeX, sizeY, UncompressedColor, OutCompressedBytes);

	return OutCompressedBytes.Num() > 0;
}

bool USimpleChatGPTLibrary::SaveBase64DataToLocalDisk(const FString& InImageBase64String, const FString& InFileName)
{
	TArray<uint8> ImageBytesData;
	if (FBase64::Decode(InImageBase64String, ImageBytesData))
	{
		if (TSharedPtr<IImageWrapper> PNGImageWrapper = FModuleManager::LoadModuleChecked<IImageWrapperModule>("ImageWrapper").CreateImageWrapper(EImageFormat::PNG))
		{
			if (PNGImageWrapper->SetCompressed(ImageBytesData.GetData(), ImageBytesData.Max()))
			{
				TArray<uint8> UncompressedBGRA;
				if (PNGImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
				{
					TArray<uint8> CompressedBytes;
					if (CompressImageArray(PNGImageWrapper->GetWidth(), PNGImageWrapper->GetHeight(), UncompressedBGRA, CompressedBytes))
					{
						return FFileHelper::SaveArrayToFile(CompressedBytes, *InFileName);
					}
				}
			}
		}

	}
	return false;
}

