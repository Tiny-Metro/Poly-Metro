// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class GridType : uint8 {
	Ground = 0 UMETA(DisplayName = "Ground"),
	Water = 1 UMETA(DisplayName = "Water"),
	Hill = 2 UMETA(DisplayName = "Hill")
};
