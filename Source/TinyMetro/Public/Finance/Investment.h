// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentData.h"
#include "InvestmentState.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "Investment.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TINYMETRO_API UInvestment : public UObject
{
	GENERATED_BODY()

public:
	void SetInvestmentData(FInvestmentData Data);
	void SetDaytime(int32 T);
	void SetPlayerState(ATinyMetroPlayerState* P);
	void SetWorld(UWorld* W);
	void SetSuccessFunction(TFunction<bool(void)> Func);
	void InvestmentSuccess();
	void InvestmentFail();
	void InitInvestment();
	void InvestmentStart();

public:
	UFUNCTION(BlueprintCallable)
	FInvestmentData GetInvestmentData() const;
	UFUNCTION(BlueprintCallable)
	bool GetIsActivate() const;
	UFUNCTION(BlueprintCallable)
	InvestmentState GetState() const;
	UFUNCTION(BlueprintCallable)
	void ActivateInvestment();
	//UFUNCTION(BlueprintCallable)
	//void DisableInvestment();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FInvestmentData InvestmentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RemainTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsActivate = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	InvestmentState State;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(BlueprintReadOnly)
	UWorld* World;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle InvestmentSuccessHandle;
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle InvestmentTimeHandle;

	TFunction<bool(void)> CheckSuccess;
};
