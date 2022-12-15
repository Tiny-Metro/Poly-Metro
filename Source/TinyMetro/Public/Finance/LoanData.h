// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoanData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FLoanData
{
	GENERATED_BODY()
	FLoanData() = default;
	FLoanData(int32 Amount, int32 DueDate, int32 Interest, float Rate) : 
		Amount(Amount), 
		DueDate(DueDate), 
		Interest(Interest), 
		Rate(Rate) {}
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DueDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Interest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rate;
};
