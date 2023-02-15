// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaneDirection.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class LaneDirection : uint8 {
	Horizontal = 0 UMETA(DisplayName = "Horizontal"),
	Vertical = 1 UMETA(DisplayName = "Vertial"),
	DiagonalL = 2 UMETA(DisplayName = "DiagonalL"),
	DiagonalR = 3 UMETA(DisplayName = "DiagonalR")
};
