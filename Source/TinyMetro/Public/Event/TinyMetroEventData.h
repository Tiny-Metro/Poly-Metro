// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TinyMetroEventData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FTinyMetroEventData
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Id;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Title;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString FlavorText;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Message;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Period;
};
