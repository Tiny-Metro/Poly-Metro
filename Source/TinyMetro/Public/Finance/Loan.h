// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LoanData.h"
#include "./GameModes/TinyMetroGameModeBase.h"
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
	void Repay();
	void CalculateInterest();
	void SetLoanData(FLoanData Data);
	void SetDaytime(int32 T);
	void SetPlayerState(ATinyMetroPlayerState* P);
	void SetWorld(UWorld* W);
	void SetAvailabilityFunction(TFunction<bool(void)> Func);

public:
	UFUNCTION(BlueprintCallable)
	FLoanData GetLoanData() const;
	UFUNCTION(BlueprintCallable)
	bool GetIsActivate() const;
	UFUNCTION(BlueprintCallable)
	bool GetAvailable();
	UFUNCTION(BlueprintCallable)
	void ActivateLoan();

protected:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FLoanData LoanData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsActivate = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsAvailable = false;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(BlueprintReadOnly)
	UWorld* World;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle LoanHandle;

	TFunction<bool(void)> CheckAvailable;

};
