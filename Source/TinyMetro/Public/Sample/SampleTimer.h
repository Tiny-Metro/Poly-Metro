// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Finance/LoanData.h"

/**
 * 
 */
class TINYMETRO_API SampleTimer
{
public:
	SampleTimer(UWorld* World, FLoanData& Data);
	~SampleTimer();

public:
	//static SampleTimer* ConstructSample(int32 Amount, int32 DueDate, float Rate);
	void Test();

protected:
	FLoanData LoanData;
	FTimerHandle TestHandle;
	UWorld* World;
};
