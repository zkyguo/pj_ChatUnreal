// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatType.h"
#include "UI_Base.generated.h"

/**
 * 
 */
UCLASS()
class PJ_CHATUNREAL_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()

protected:
	UUserWidget* GetUserWidgetsClass(TSubclassOf<UUserWidget> widgetClass);

	template<class T>
	T* GetUserWidgetsClass(TSubclassOf<UUserWidget> widgetClass)
	{
		return Cast<T>(GetUserWidgetsClass(widgetClass));
	}
};

