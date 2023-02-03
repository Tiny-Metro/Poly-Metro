// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class  GridStationStructure : uint8
{
	Empty = 0 UMETA(DisplayName = "Empty"),
	Station = 1 UMETA(DisplayName = "Station")
};
