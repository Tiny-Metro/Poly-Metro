// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TinyMetroEffectSound.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class TinyMetroEffectSound : uint8
{
	StationSpawn = 0 UMETA(DisplayName = "StationSpawn"),
	SpawnTrain = 1 UMETA(DisplayName = "SpawnTrain"),
	SpawnSubtrain = 2 UMETA(DisplayName = "SpawnSubtrain"),
	TrainUpgrade = 3 UMETA(DisplayName = "TrainUpgrade"),
	StationUpgrade = 4 UMETA(DisplayName = "StationUpgrade"),
	OccurEvent = 5 UMETA(DisplayName = "OccurEvent"),
	ButtonClick = 6 UMETA(DisplayName = "ButtonClick"),
	TabMove = 7 UMETA(DisplayName = "TabMove"),
	Pause = 8 UMETA(DisplayName = "Pause"),
	Unpause = 9 UMETA(DisplayName = "Unpause")
	
};
