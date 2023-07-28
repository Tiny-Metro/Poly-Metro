// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SubtrainStatistics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FSubtrainStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalPurchaseCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalPlacementCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalShiftCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalRetrievalCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalUpgradeCount = 0;
};
