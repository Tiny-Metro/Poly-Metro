// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentData.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "Investment.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UInvestment : public UObject
{
	GENERATED_BODY()

public:
	void SetInvestmentData(FInvestmentData Data);
	void SetDaytime(int32 T);
	void SetPlyaerState(ATinyMetroPlayerState* P);
	void SetWorld(UWorld* W);
	void InvestmentSuccess();
	void InvestmentFail();

public:
	UFUNCTION(BlueprintCallable)
	FInvestmentData GetInvestmentData() const;
	UFUNCTION(BlueprintCallable)
	bool GetIsActivate() const;
	UFUNCTION(BlueprintCallable)
	void ActivateInvestment();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Data")
	FInvestmentData InvestmentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	bool IsActivate = false;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(BlueprintReadOnly)
	UWorld* World;
};
