// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TrainDirection.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class TrainDirection : uint8
{
	//Empty = 0 UMETA(DisplayName = "Empty"),
	Down = 0 UMETA(DisplayName = "Down"),
	Up = 1 UMETA(DisplayName = "Up")
	
};
