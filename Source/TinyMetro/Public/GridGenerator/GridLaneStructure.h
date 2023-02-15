// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class GridLaneStructure : uint8
{
	Empty = 0 UMETA(DisplayName = "Empty"),
	Lane = 1 UMETA(DisplayName = "Lane"),
	Bridge = 2 UMETA(DisplayName = "Bridge"),
	Turnel = 3 UMETA(DisplayName = "Turnel")
};
