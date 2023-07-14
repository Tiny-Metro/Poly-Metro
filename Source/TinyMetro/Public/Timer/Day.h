// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Day.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class Day : uint8
{
	Monday = 0 UMETA(DisplayName = "Monday"),
	Tuesday = 1 UMETA(DisplayName = "Tuesday"),
	Wednesday = 2 UMETA(DisplayName = "Wednesday"),
	Thursday = 3 UMETA(DisplayName = "Thursday"),
	Friday = 4 UMETA(DisplayName = "Friday"),
	Saturday = 5 UMETA(DisplayName = "Saturday"),
	Sunday = 6 UMETA(DisplayName = "Sunday")
	
};
