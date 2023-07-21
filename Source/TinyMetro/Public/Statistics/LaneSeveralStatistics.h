// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaneSeveralStatistics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FLaneSeveralStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalArrivePassenger = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeeklyArrivePassenger = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsingBridgeCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsingTunnelCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TransferStationCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ServiceStationCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalModifyAndReduceCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AverageComplain = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ServiceTrainAndSubtrainCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsCircularLane = false;

};
