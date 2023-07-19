// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../GridGenerator/GridCellData.h"
#include "../Station/StationType.h"
#include "../Timer/Timestamp.h"
#include "StationSpawnData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FStationSpawnData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FGridCellData GridCellData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	StationType Type;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 StationId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTimestamp Timestamp;
};
