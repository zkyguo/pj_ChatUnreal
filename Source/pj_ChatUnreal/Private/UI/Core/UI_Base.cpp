// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Core/UI_Base.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

UUserWidget* UUI_Base::GetUserWidgetsClass(TSubclassOf<UUserWidget> widgetClass)
{
	TArray<UUserWidget*> UserWidgets;
	UWidgetBlueprintLibrary::GetAllWidgetsOfClass(GetWorld(), UserWidgets, widgetClass, true);

	if(UserWidgets.Num() > 0)
	{
		return UserWidgets[0];
	}

	return nullptr;
}
