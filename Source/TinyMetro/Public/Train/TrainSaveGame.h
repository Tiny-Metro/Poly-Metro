// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TrainInfo.h"
#include "TrainStatus.h"
#include "../Station/Passenger.h"
#include "../Station/StationInfo.h"
#include "TrainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTrainSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FTrainInfo TrainInfo;
	UPROPERTY(BlueprintReadWrite)
	TMap<int32, FPassenger> Passenger;
	UPROPERTY(BlueprintReadWrite)
	FStationInfo CurrentStation;
	UPROPERTY(BlueprintReadWrite)
	FStationInfo NextStation;
	UPROPERTY(BlueprintReadWrite)
	TrainStatus Status;
};
