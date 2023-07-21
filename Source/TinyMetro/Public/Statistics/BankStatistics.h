// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BankStatistics.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FBankStatistics
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalUsingLoanCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalRepayMoney = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalLoanMoney = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalRepayInterestMoney = 0;
};
