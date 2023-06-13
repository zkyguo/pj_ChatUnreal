// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "SimpleChatGPTLibrary.generated.h"

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class USimpleChatGPTLibrary : public UObject
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "ChatGPT")
	static UTexture2D* Base64ToTexture2D(const FString& InImageBase64String);

	UFUNCTION(BlueprintCallable, Category = "ChatGPT")
	static bool SaveTexture2DToLocalDisk(UTexture2D* Texture2D, const FString &InFileName);

	UFUNCTION(BlueprintCallable, Category = "ChatGPT")
	static bool CompressImageArray(int32 sizeX, int32 sizeY,const TArray<uint8> &InUncompressedBGRA, TArray<uint8> &OutCompressedBytes);

	UFUNCTION(BlueprintCallable, Category = "ChatGPT")
	static bool SaveBase64DataToLocalDisk(const FString& InImageBase64String, const FString& InFileName);
};
