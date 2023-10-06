// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TrainType.h"
#include "TrainInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FTrainInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ServiceLaneId = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TrainType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SubtrainCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalBoardPassenger = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeeklyBoardPassenger = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShiftCount = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsUpgrade = false;
};
