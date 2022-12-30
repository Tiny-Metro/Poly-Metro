// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemType.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ItemType : uint8
{
	Train = 0 UMETA(DisplayName = "Train"),
	Subtrain = 1 UMETA(DisplayName = "Subtrain"),
	Lane = 2 UMETA(DisplayName = "Lane"),
	Tunnel = 3 UMETA(DisplayName = "Tunnel"),
	Bridge = 4 UMETA(DisplayName = "Bridge"),
	UpgradeTrain = 5 UMETA(DisplayName = "UpgradeTrain"),
	UpgradeSubtrain = 6 UMETA(DisplayName = "UpgradeSubtrain"),
	UpgradeStation = 7 UMETA(DisplayName = "UpgradeStation"),
	
};
