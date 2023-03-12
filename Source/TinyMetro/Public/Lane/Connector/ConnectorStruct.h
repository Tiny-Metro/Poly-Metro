// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Connector.h"
#include "ConnectorStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FConnectorStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FConnectorPoint> PointArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AConnector* ConnectorRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	enum class EConnector : uint8 {
		Bridge = 0 UMETA(DisplayName = "Bridge"),
		Tunnel = 1 UMETA(DisplayName = "Tunnel")
	};

};