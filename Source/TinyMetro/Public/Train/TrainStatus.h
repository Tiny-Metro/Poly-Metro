// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class TrainStatus : uint8
{
	Spawn = 0 UMETA(DisplayName = "Spawn"),
	Run = 1 UMETA(DisplayName = "Running"),
	GetOn = 2 UMETA(DisplayName = "GetOn"),
	GetOff = 3 UMETA(DisplayName = "GetOff")
};
