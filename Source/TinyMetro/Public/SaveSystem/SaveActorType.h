// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class SaveActorType : uint8
{
	StationManager = 0 UMETA(DisplayName = "StationMananger"),
	Station = 1 UMETA(DisplayName = "Station"),
	LaneManager = 2 UMETA(DisplayName = "LaneManager"),
	Lane = 3 UMETA(DisplayName = "Lane"),
	TrainManager = 4 UMETA(DisplayName = "TrainManager"),
	Train = 5 UMETA(DisplayName = "Train"),
	StatisticsManager = 6 UMETA(DisplayName = "StatisticsManager"),
	Policy = 6 UMETA(DisplayName = "Policy")
};
