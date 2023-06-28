// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class TrainStatus : uint8
{
	Run = 0 UMETA(DisplayName = "Running"),
	GetOn = 1 UMETA(DisplayName = "GetOn"),
	GetOff = 2 UMETA(DisplayName = "GetOff")
};
