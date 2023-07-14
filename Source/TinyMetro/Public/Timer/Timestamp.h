// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Day.h"
#include "Timestamp.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FTimestamp
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Date = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Week = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	Day DayoftheWeek = Day::Monday;
};
