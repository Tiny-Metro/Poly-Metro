// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Timer/Timestamp.h"
#include "TinyMetroEventData.h"
#include "TinyMetroEventOccurData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FTinyMetroEventOccurData
{
	GENERATED_USTRUCT_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimestamp Timestamp;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTinyMetroEventData EventData;
	
};
