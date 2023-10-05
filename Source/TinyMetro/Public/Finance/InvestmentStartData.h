// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Timer/Timestamp.h"
#include "../Statistics/DefaultStatistics.h"
#include "../Statistics/LaneStatistics.h"
#include "../Statistics/ShopStatistics.h"
#include "../Statistics/BankStatistics.h"
#include "../Station/StationInfo.h"
#include "../Train/TrainInfo.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentStartData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FInvestmentStartData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FTimestamp Timestamp;
	UPROPERTY(BlueprintReadWrite)
	FDefaultStatistics DefaultStatistics;
	UPROPERTY(BlueprintReadWrite)
	FLaneStatistics LaneStatistics;
	UPROPERTY(BlueprintReadWrite)
	FShopStatistics ShopStatistics;
	UPROPERTY(BlueprintReadWrite)
	FBankStatistics BankStatistics;
	UPROPERTY(BlueprintReadWrite)
	TArray<FStationInfo> StationInfoArr;
	UPROPERTY(BlueprintReadWrite)
	TArray<FTrainInfo> TrainInfoArr;
};
