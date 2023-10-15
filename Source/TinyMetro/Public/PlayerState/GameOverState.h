// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class GameOverState : uint8
{
	Playing = 0 UMETA(DisplayName = "Playing"),
	Complain = 1 UMETA(DisplayName = "Complain"),
	Bankruptcy = 2 UMETA(DisplayName = "Bankruptcy")
};
