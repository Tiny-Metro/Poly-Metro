// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaneSeveralStatistics.h"
#include "LaneStatistics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FLaneStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalLaneCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalModifyAndDeleteCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int, FLaneSeveralStatistics> Lanes = {
		{ 1, FLaneSeveralStatistics() },
		{ 2, FLaneSeveralStatistics() },
		{ 3, FLaneSeveralStatistics() },
		{ 4, FLaneSeveralStatistics() },
		{ 5, FLaneSeveralStatistics() },
		{ 6, FLaneSeveralStatistics() },
		{ 7, FLaneSeveralStatistics() },
		{ 8, FLaneSeveralStatistics() }
	};
};
