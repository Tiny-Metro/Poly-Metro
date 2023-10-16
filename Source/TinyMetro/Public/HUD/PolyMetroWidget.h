// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HUDEnums.h"
#include "HUDStructs.h"
#include "Engine/TextRenderActor.h"  // If you're working with TextRender components
#include "Internationalization/Text.h"  // For FText

//#include "HUDManager.h"
class AHUDManager;
#include "PolyMetroWidget.generated.h"


UCLASS(Blueprintable)
class TINYMETRO_API UPolyMetroWidget : public UUserWidget
{
	
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WidgetName;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTextComponent> TextComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTextComponent> DynamicTextComponents;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FImageComponent> ImageComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPolyMetroWidget*> Widgets;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSlateFontInfo DefaultFont;

public:
	UFUNCTION(BlueprintCallable)
	void AssignTextComponent(FTextComponent NewTextComponent);

	UFUNCTION(BlueprintCallable)
	void AssignImageComponent(FImageComponent NewImageComponent);
	
	UFUNCTION(BlueprintCallable)
	void UpdateTextComponents(AHUDManager* hudManager);

	UFUNCTION(BlueprintCallable)
	void UpdateImageComponents(AHUDManager* hudManager);

	UFUNCTION(BlueprintCallable)
	void UpdateWidgets(AHUDManager* hudManager);

	/*
public:
	UFUNCTION(BlueprintCallable)
	bool IsValid();
	*/


};