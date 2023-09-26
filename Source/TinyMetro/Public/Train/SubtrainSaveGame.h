// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TrainInfo.h"
#include "../Station/Passenger.h"
#include "SubtrainSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API USubtrainSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FTrainInfo TrainInfo;
	UPROPERTY(BlueprintReadWrite)
	TMap<int32, FPassenger> Passenger;
	UPROPERTY(BlueprintReadWrite)
	int32 OwnerTrainId;
	UPROPERTY(BlueprintReadWrite)
	FRotator Rotation;
};
