// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LanePoint.h"
#include "LaneSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ULaneSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FLanePoint> LaneArray;

	UPROPERTY()
	bool IsCircularLane;

	UPROPERTY()
	TArray <int32> StationPoint;

	UPROPERTY()
	TArray <FIntPoint> PointArray;

	UPROPERTY()
	TArray<FVector> LaneLocation;

	//Delay Removing
	UPROPERTY()
	bool DoesStationsToBeRemovedAtStart;

	UPROPERTY()
	bool DoesStationsToBeRemovedAtEnd;

	UPROPERTY()
	bool DoesLaneToBeRemoved;

	UPROPERTY()
	TArray<int32> StationsToBeRemovedAtStart;

	UPROPERTY()
	TArray<int32> StationsToBeRemovedAtEnd;

	UPROPERTY()
	TArray<int32> StationPointBeforeRemovedStart;

	UPROPERTY()
	TArray<int32> StationPointBeforeRemovedEnd;
};
