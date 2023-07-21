// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TrainStatistics.h"
#include "SubtrainStatistics.h"
#include "BridgeStatistics.h"
#include "TunnelStatistics.h"
#include "ShopStatistics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FShopStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalUsingMoney = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalPurchaseCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTrainStatistics TrainStatistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSubtrainStatistics SubtrainStatistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBridgeStatistics BridgeStatistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTunnelStatistics TunnelStatistics;
};
