// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConnectorType.h"
#include "BridgeTunnel.h"
#include "ConnectorData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct TINYMETRO_API FConnectorData
{
	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FIntPoint> PointArr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ConnectorType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TWeakObjectPtr<ABridgeTunnel> ConnectorREF;
};