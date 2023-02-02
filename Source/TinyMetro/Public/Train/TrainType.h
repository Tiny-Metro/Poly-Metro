// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class TrainType : uint8
{
	//Empty = 0 UMETA(DisplayName = "Empty"),
	Train = 0 UMETA(DisplayName = "Train"),
	SubTrain = 1 UMETA(DisplayName = "SubTrain")
};
