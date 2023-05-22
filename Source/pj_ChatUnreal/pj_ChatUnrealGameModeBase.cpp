// Copyright Epic Games, Inc. All Rights Reserved.


#include "pj_ChatUnrealGameModeBase.h"
#include "ChatHUD.h"
#include "ChatPawn.h"
#include "ChatPlayerController.h"

Apj_ChatUnrealGameModeBase::Apj_ChatUnrealGameModeBase()
{
	HUDClass = AChatHUD::StaticClass();
	DefaultPawnClass = AChatPawn::StaticClass();
	PlayerControllerClass = AChatPlayerController::StaticClass();
}
