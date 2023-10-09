// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoanData.h"
#include "LoanSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ULoanSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FLoanData LoanData;
	UPROPERTY()
	int32 Balance;
	UPROPERTY()
	int32 RepayPerWeek;
	UPROPERTY()
	int32 RemainTime;
	UPROPERTY()
	bool IsActivate = false;
	UPROPERTY()
	bool IsAvailable = false;
	
};
