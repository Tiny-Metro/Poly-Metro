// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaneType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class LaneType: uint8 {
	Normal = 0 UMETA(DisplayName = "Normal"),
	Bridge = 1 UMETA(DisplayName = "Bridge"),
	Tunnel = 2 UMETA(DisplayName = "Tunnel")
};
