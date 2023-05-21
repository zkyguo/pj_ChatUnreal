// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once
#include "SimpleChatGPTType.h"

FChatGPTCompletionParam::FChatGPTCompletionParam()
{
	Mode = EChatGPTModel::DAVINCI_003;
	MaxToken = 300;
	Temperature = 0.5f;
	Top_p = 1.0f;
	N = 1.0f;
	bStream = false;
	Stop = TEXT("\n");
}
