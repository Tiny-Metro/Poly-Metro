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
	UInvestment();
	static UInvestment* CreateInvestment(FString ScriptFileName, class UInvestmentLuaState* LuaState, UWorld* WorldContextObject);
	UFUNCTION()
	void InitInvestment();
	UFUNCTION()
	void ResetInvestment();

	// Investment processing
	UFUNCTION(BlueprintCallable)
	void InvestmentStart();
	UFUNCTION(BlueprintCallable)
	InvestmentState InvestmentProcess();
	UFUNCTION(BlueprintCallable)
	InvestmentState InvestmentFinish();
	UFUNCTION(BlueprintCallable)
	void InvestmentSuccess();
	
	// Investment appearance
	UFUNCTION(BlueprintCallable)
	bool GetAppearable() const;

public:
	UFUNCTION(BlueprintCallable)
	FInvestmentData GetInvestmentData() const;
	UFUNCTION(BlueprintCallable)
	InvestmentState GetState() const;

	// Call by Bank::DailyTask
	UFUNCTION(BlueprintCallable)
	void NotifyDailyTask();

protected:
	UPROPERTY()
	UWorld* WorldRef;
	UPROPERTY()
	FString ScriptFileName;
	UPROPERTY()
	FString ScriptDirectory;

	class UInvestmentLuaState* LuaState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FInvestmentData InvestmentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RemainTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	InvestmentState State = InvestmentState::Ready;
};
