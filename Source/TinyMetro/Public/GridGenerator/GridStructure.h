// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridStructure.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class GridStructure : uint8
{
	Empty = 0 UMETA(DisplayName = "Empty"),
	Station = 1 UMETA(DisplayName = "Station"),
	Lane = 2 UMETA(DisplayName = "Lane"),
	Bridge = 3 UMETA(DisplayName = "Bridge"),
	Turnel = 4 UMETA(DisplayName = "Turnel")
	
};
