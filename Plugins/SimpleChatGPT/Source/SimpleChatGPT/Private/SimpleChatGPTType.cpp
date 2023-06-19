// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "SimpleChatGPTType.h"

FChatGPTParamBase::FChatGPTParamBase()
{
	Mode = EChatGPTModel::TEXT_DAVINCI_003;
	MaxToken = 1024;
	Temperature = 0.5f;
}

FChatGPTCompletionParam::FChatGPTCompletionParam()
{
	Mode = EChatGPTModel::TEXT_DAVINCI_003;
	Top_p = 1.0f;
	N = 1.0f;
	bStream = false;
}

FChatGPTCompletionContextParam::FChatGPTCompletionContextParam()
{

}

FChatGPTContextMessage::FChatGPTContextMessage()
{
	Role = EChatGPTRole::USER;
}

FChatGPTChoice::FChatGPTChoice()
{
	Index = INDEX_NONE;
}

FChatGPTUsage::FChatGPTUsage()
{
	PromptTokens = INDEX_NONE;
	CompletionTokens = INDEX_NONE;
	TotalTokens = INDEX_NONE;
}

FChatGPTCompletionResponse::FChatGPTCompletionResponse()
{
	Created = INDEX_NONE;
	Mode = EChatGPTModel::NONE;
}

FChatGPTImageGenerationParam::FChatGPTImageGenerationParam()
{
	ImageNumber = 1;
	ImageSizeType = EChatGPTImageSizeType::IMG_256x256;
	ImageEncodingType = EChatGPTImageEncodingType::IMG_URL;
}


