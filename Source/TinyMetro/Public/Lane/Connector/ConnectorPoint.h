// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ConnectorPoint.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FConnectorPoint
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntPoint> PointArray;

};
