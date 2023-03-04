// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdjItem.h"
#include "AdjArrayItem.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FAdjArrayItem
{

	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TArray<FAdjItem> AdjItems;
};
