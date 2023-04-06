// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LaneType.h"
#include "LaneDirection.h"
#include "Components/SplineMeshComponent.h"
#include "LanePoint.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct TINYMETRO_API FLanePoint {

	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Coordination;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsStation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsBendingPoint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsThrough;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LanePosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	LaneType LaneType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	LaneDirection LaneDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USplineMeshComponent*> MeshArray;
};