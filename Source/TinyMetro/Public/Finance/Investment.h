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
	void InitLuaState();
	UFUNCTION()
	void InitInvestment();
	UFUNCTION()
	void ResetInvestment();

	UFUNCTION()
	bool GetAppearance();

	UFUNCTION(BlueprintCallable)
	void InvestmentStart();
	UFUNCTION(BlueprintCallable)
	void InvestmentProcess();
	UFUNCTION()
	void InvestmentSuccess();
	UFUNCTION()
	void InvestmentFail();
	
public:
	UFUNCTION(BlueprintCallable)
	FInvestmentData GetInvestmentData() const;
	UFUNCTION(BlueprintCallable)
	InvestmentState GetInvestmentState() const;
	// Call by Bank::DailyTask
	UFUNCTION(BlueprintCallable)
	void NotifyDailyTask();

	//UFUNCTION(BlueprintCallable)
	//void DisableInvestment();


protected:
	UPROPERTY()
	UWorld* WorldRef;
	UPROPERTY(VisibleAnywhere)
	FString ScriptFileName;
	UPROPERTY()
	FString ScriptDirectory;
	UPROPERTY()
	class UInvestmentLuaState* LuaState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FInvestmentData InvestmentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 RemainTime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	float ElapseTime = 0.0f;


	// Mutex
	TSharedPtr<FCriticalSection> MutexKey = MakeShared<FCriticalSection>();
};
