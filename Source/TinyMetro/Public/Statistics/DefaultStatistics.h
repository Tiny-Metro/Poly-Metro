// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DefaultStatistics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FDefaultStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalArrivePassenger = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeeklyArrivePassenger = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalProfit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeeklyProfit = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalSpending = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeeklySpending = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AverageComplain = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ServiceStationCount = 0;

};
