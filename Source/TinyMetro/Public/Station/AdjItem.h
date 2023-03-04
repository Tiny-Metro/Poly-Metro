// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StationType.h"
#include "AdjItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FAdjItem
{
	GENERATED_BODY()


	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 StationId;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	StationType StationType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float Length;

};
