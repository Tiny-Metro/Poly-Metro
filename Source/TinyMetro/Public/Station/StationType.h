// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StationType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class StationType : uint8{
	Circle = 0 UMETA(DisplayName = "Circle"),
	Triangle = 1 UMETA(DisplayName = "Triangle"),
	Square = 2 UMETA(DisplayName = "Square"),
	Cross = 3 UMETA(DisplayName = "Cross"),
	Lozenge = 4 UMETA(DisplayName = "Lozenge"),
	Almond = 5 UMETA(DisplayName = "Almond"),
	Diamond = 6 UMETA(DisplayName = "Diamond"),
	Pentagon = 7 UMETA(DisplayName = "Pentagon"),
	Star = 8 UMETA(DisplayName = "Star"),
	Fan = 9 UMETA(DisplayName = "Fan")
	
};
