// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConnectorType.generated.h"

/**
 *
 */
UENUM(BlueprintType)
enum class ConnectorType : uint8 {
	Bridge = 0 UMETA(DisplayName = "Bridge"),
	Tunnel = 1 UMETA(DisplayName = "Tunnel")
};