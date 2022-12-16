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
	FLoanData(int32 Amount, int32 AutoRepay, float Rate, FString Message) : 
		Amount(Amount), 
		AutoRepay(AutoRepay),
		Interest(0), 
		Rate(Rate),
		Message(Message) { }
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AutoRepay;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Interest;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;
};
