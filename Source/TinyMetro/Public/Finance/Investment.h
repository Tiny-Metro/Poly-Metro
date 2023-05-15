// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentData.h"
#include "InvestmentState.h"
#include "InvestmentResult.h"
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
	static UInvestment* CreateInvestment(FInvestmentData Data, int32 Daytime, TFunction<void(void)> Start, TFunction<void(void)> Success, TFunction<void(void)> Fail, TFunction<InvestmentState(void)> Check);
	void SetDaytime(int32 T);
	void SetInvestmentData(FInvestmentData Data);
	void SetAcceptAction(TFunction<void(void)> Action);
	void SetSuccessAction(TFunction<void(void)> Action);
	void SetFailAction(TFunction<void(void)> Action);
	void SetStateCheckFunction(TFunction<InvestmentState(void)> Check);
	void InitInvestment();

	UFUNCTION(BlueprintCallable)
	void InvestmentStart();
	
public:
	UFUNCTION(BlueprintCallable)
	FInvestmentData GetInvestmentData() const;
	UFUNCTION(BlueprintCallable)
	InvestmentState GetState() const;
	UFUNCTION(BlueprintCallable)
	InvestmentState CheckInvestmentProcess(float DeltaTime);
	UFUNCTION(BlueprintCallable)
	void NotifyDailyTask();
	//UFUNCTION(BlueprintCallable)
	//void DisableInvestment();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FInvestmentData InvestmentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RemainTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	float ElapseTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	InvestmentState State = InvestmentState::Ready;

	TFunction<void(void)> AcceptAction;
	TFunction<void(void)> SuccessAction;
	TFunction<void(void)> FailAction;
	TFunction<InvestmentState(void)> ConditionCheckFunction;
};
