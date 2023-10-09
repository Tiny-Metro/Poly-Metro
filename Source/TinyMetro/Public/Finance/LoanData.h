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
	FLoanData(int32 Id, int32 Amount, float Rate, int32 DueDateWeek, FString Message) :
		Id(Id),
		Amount(Amount),
		Rate(Rate),
		DueDateWeek(DueDateWeek),
		Message(Message) { }
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Id = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DueDateWeek;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;
};
