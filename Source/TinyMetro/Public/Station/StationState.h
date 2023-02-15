// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StationState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class StationState : uint8
{
	Inactive = 0 UMETA(DisplayName = "Inactive"),
	Active = 1 UMETA(DisplayName = "Active"),
	Destroyed = 2 UMETA(DisplayName = "Destroyed")
	
};
