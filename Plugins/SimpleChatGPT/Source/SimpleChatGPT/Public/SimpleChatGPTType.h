// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "SimpleChatGPTType.generated.h"

UENUM(BlueprintType)
enum ESimpleChatGPTMode : uint8
{
	
	DAVINCI_003 UMETA(displayName = "text_davinci-003"),
	GPT_MAX,
};

USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTCompletionParam
{
	GENERATED_BODY()

	FChatGPTCompletionParam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="CompletionParam")
	ESimpleChatGPTMode mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	int32 MaxToken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	float Temperature;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	float Top_p;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	float N;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	bool bStream;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	FString Stop;
};
