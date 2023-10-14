// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HUDEnums.h"
#include "HUDStructs.h"

#include "PolyMetroWidget.generated.h"


UCLASS(Blueprintable)
class TINYMETRO_API UPolyMetroWidget : public UUserWidget
{
	
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString WidgetName;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Text")
	TMap<FString, FString> TextBlockMap;

	UFUNCTION(BlueprintCallable)
	FString GetTextByName(FString Name);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTextComponent> TextComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FImageComponent> ImageComponents;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UPolyMetroWidget*> Widgets;

};