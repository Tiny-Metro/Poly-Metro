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
	Rectangle = 2 UMETA(DisplayName = "Rectangle"),
	Cross = 3 UMETA(DisplayName = "Cross"),
	Rhombus = 4 UMETA(DisplayName = "Rhombus"),
	Oval = 5 UMETA(DisplayName = "Oval"),
	Diamond = 6 UMETA(DisplayName = "Diamond"),
	Pentagon = 7 UMETA(DisplayName = "Pentagon"),
	Star = 8 UMETA(DisplayName = "Star"),
	Fan = 9 UMETA(DisplayName = "Fan")
	
};
