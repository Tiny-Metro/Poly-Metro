// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridType.h"
#include "GridStructure.h"
#include "GridStationStructure.h"
#include "GridLaneStructure.h"
#include "GridCellData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FGridCellData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Index;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint WorldCoordination;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector WorldLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GridType GridType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GridStructure GridStructure;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GridStationStructure StationInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	GridLaneStructure LaneInfo;

};
