// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "SimpleChatGPTType.generated.h"

UENUM(BlueprintType)
enum class EChatGPTModel : uint8
{
	NONE UMETA(DisplayName = "Invalid"),
	DAVINCI_003 UMETA(DisplayName = "text_davinci-003"),
	GPT_MAX,
};

USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTCompletionParam
{
	GENERATED_BODY()

	FChatGPTCompletionParam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="CompletionParam")
	EChatGPTModel Mode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionParam")
	FString Prompt;
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

USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTLogprobs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTLogprobs")
	TArray<FString> Tokens;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTLogprobs")
	TArray<double> Tokens_logprobs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTLogprobs")
	TArray<int32> Text_offset;
};

USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTChoice
{
	GENERATED_BODY()

	FChatGPTChoice();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTChoice")
	FString Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTChoice")
	int32 Index;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTChoice")
	FChatGPTLogprobs logProbs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTChoice")
	FString FinishReason;
};

USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTUsage
{
	GENERATED_BODY()

	FChatGPTUsage();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTUsage")
	int32 PromptTokens; //Cost of token of request

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTUsage")
	int32 CompletionTokens; //Cost of token of response

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChatGPTUsage")
	int32 TotalTokens; //Cost of total token 
};


USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTCompletionResponse
{
	GENERATED_BODY()

	FChatGPTCompletionResponse();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionResponse")
	FString ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionResponse")
	FString Object;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionResponse")
	int64 Created;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionResponse")
	EChatGPTModel Mode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionResponse")
	TArray<FChatGPTChoice> Choices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CompletionResponse")
	FChatGPTUsage Usage;
};

