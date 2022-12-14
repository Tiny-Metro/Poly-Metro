// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FInvestmentData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Award;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Reward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;
};
