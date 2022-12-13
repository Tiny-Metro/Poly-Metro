// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GamePlayInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FGamePlayInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PlayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Sales;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Profit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SalesInWeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ProfitInWeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ComplainAverage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Passenger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsingTrain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsingLane;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UsingTunnel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConnectedStation;
};
