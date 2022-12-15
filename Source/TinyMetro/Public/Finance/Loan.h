// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoanData.h"
#include "Loan.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ULoan : public UObject
{
	GENERATED_BODY()

public:
	void Test();

protected:
	FLoanData LoanData;
	FTimerHandle TestHandle;
	UWorld* World;
};
