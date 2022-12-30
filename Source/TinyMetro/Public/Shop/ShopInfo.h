// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ShopInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FShopInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostTrain = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostSubtrain = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostLane = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostTunnel = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostBridge = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostUpgradeTrain = 300;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostUpgradeSubtrain = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CostUpgradeStation = 500;
};
