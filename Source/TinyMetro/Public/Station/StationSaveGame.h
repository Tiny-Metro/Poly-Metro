// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Passenger.h"
#include "StationSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UStationSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	TArray<FPassenger> Passenger;
	UPROPERTY(BlueprintReadWrite)
	float ComplainCurrent;
	UPROPERTY(BlueprintReadWrite)
	float PassengerSpawnCurrent;
	UPROPERTY(BlueprintReadWrite)
	int32 SpawnDay = 0;
	UPROPERTY(BlueprintReadWrite)
	bool IsUpgrade;

};
