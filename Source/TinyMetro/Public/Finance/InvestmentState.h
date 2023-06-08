// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class InvestmentState : uint8
{
	Ready = 0 UMETA(DisplayName = "Ready"),
	Processing = 1 UMETA(DisplayName = "Processing"),
	Success = 2 UMETA(DisplayName = "Success"),
	Fail = 3 UMETA(DisplayName = "Fail")
	
};
