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
	None = 0 UMETA(DisplayName = "None "),
	Circle = 1 UMETA(DisplayName = "Circle"),
	Triangle = 2 UMETA(DisplayName = "Triangle"),
	Rectangle = 3 UMETA(DisplayName = "Rectangle"),
	Cross = 4 UMETA(DisplayName = "Cross"),
	Rhombus = 5 UMETA(DisplayName = "Rhombus"),
	Oval = 6 UMETA(DisplayName = "Oval"),
	Diamond = 7 UMETA(DisplayName = "Diamond"),
	Pentagon = 8 UMETA(DisplayName = "Pentagon"),
	Star = 9 UMETA(DisplayName = "Star"),
	Fan = 10 UMETA(DisplayName = "Fan")
	
};
