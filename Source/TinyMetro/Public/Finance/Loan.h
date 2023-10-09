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
UCLASS(Blueprintable)
class TINYMETRO_API ULoan : public UObject
{
	GENERATED_BODY()

public:
	void SetLoanData(FLoanData Data);
	void SetPlayerState(ATinyMetroPlayerState* P);
	void SetWorld(UWorld* W);
	void SetStatisticsManager(class AStatisticsManager* S);
	void SetSaveManager(class ATMSaveManager* S);
	void SetAvailabilityFunction(TFunction<bool(void)> Func);
	void InitLoan(FLoanData Data);

public:
	UFUNCTION(BlueprintCallable)
	FLoanData GetLoanData() const;
	UFUNCTION(BlueprintCallable)
	bool GetIsActivate() const;
	UFUNCTION(BlueprintCallable)
	bool GetAvailable();
	UFUNCTION(BlueprintCallable)
	void ActivateLoan();
	void DisableLoan();
	UFUNCTION(BlueprintCallable)
	void RepayAll();
	void Repay();

	// Weekly logic (Call by Bank actor)
	UFUNCTION()
	void NotifyWeeklyTask();

	// Save & Load (Call by Bank actor)
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

protected:
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(BlueprintReadOnly)
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY(VisibleAnywhere)
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY(BlueprintReadOnly)
	UWorld* WorldContextReference;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FLoanData LoanData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Balance; // Balance of loan
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RepayPerWeek; // Repay amount per week
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RemainTime; // Remainder week
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsActivate = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsAvailable = false;

	TFunction<bool(void)> CheckAvailable;

};
