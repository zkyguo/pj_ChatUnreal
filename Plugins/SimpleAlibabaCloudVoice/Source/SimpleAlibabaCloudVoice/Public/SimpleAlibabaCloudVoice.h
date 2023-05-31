// Copyright (C) RenZhai.2023.All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

class FSimpleAlibabaCloudVoiceModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
};
