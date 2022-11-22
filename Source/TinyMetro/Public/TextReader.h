// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TextReader.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTextReader : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Custom", meta = (Keywords = "LoadTxt"))
	static bool LoadMapText(FString FileName, FString& OutText);
};
