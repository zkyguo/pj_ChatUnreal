// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "GenericPlatform/GenericPlatform.h"
#include "SimpleChatGPTType.generated.h"


UENUM(BlueprintType)
enum class EChatGPTImageSizeType : uint8
{
	IMG_256x256  UMETA(DisplayName = "256x256"),
	IMG_512x512 UMETA(DisplayName = "512x512"),
	IMG_1024x1024 UMETA(DisplayName = "1024x1024")
};

UENUM(BlueprintType)
enum class EChatGPTImageEncodingType : uint8
{
	IMG_URL  UMETA(DisplayName = "URL"),
	IMG_Base64 UMETA(DisplayName = "Base64")
};

UENUM(BlueprintType)
enum class EChatGPTProtocol : uint8
{
	ChatGPT_TEXT  UMETA(DisplayName = "Chat Text"),
	ChatGPT_IMAGE UMETA(DisplayName = "Chat Image")
};

UENUM(BlueprintType)
enum class EChatGPTModel : uint8
{
	NONE UMETA(DisplayName = "Invalid"),
	TEXT_DAVINCI_003 UMETA(DisplayName = "text-davinci-003"),
	TEXT_DAVINCI_002 UMETA(DisplayName = "text-davinci-002"),
	CODE_DAVINCI_002 UMETA(DisplayName = "code-davinci-002"),

	GPT_3_5_TURBO UMETA(DisplayName = "gpt-3.5-turbo"),
	GPT_4 UMETA(DisplayName = "gpt-4"),
	GPT_4_32k UMETA(DisplayName = "gpt-4-32k"),

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

USTRUCT(BlueprintType)
struct SIMPLECHATGPT_API FChatGPTImageGenerationParam
{
	GENERATED_BODY()

	FChatGPTImageGenerationParam();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	FString Prompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	int32 ImageNumber;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	EChatGPTImageSizeType ImageSizeType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Image")
	EChatGPTImageEncodingType ImageEncodingType;
};
