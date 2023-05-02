// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServiceData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FServiceData
{
public:
	GENERATED_USTRUCT_BODY()
	
	FServiceData() : ServiceLevel(-1), DailyComplain(0), WeeklyCost(0) {}
	FServiceData(int32 ServiceLevel, int32 DailyComplain, int32 WeeklyCost) : 
		ServiceLevel(ServiceLevel),
		DailyComplain(DailyComplain),
		WeeklyCost(WeeklyCost) {}

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 ServiceLevel;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 DailyComplain;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyCost;
};
