// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentResult.h"
#include "InvestmentData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FInvestmentData
{
	GENERATED_BODY()
	FInvestmentData() = default;
	FInvestmentData(FString Title, FString Content, int32 TimeRequire, FString DepositMessage, FString AwardMessage, FString RewardMessage) :
		Title(Title),
		Content(Content),
		TimeRequire(TimeRequire),
		DepositMessage(DepositMessage),
		AwardMessage(AwardMessage),
		RewardMessage(RewardMessage){ }

	// Investmet title
	UPROPERTY(BlueprintReadOnly)
	FString Title;
	// Detail explain
	UPROPERTY(BlueprintReadOnly)
	FString Content;
	UPROPERTY(BlueprintReadOnly)
	int32 TimeRequire;
	// Deposit of investment
	UPROPERTY(BlueprintReadOnly)
	FString DepositMessage;
	// Investment success
	UPROPERTY(BlueprintReadOnly)
	FString AwardMessage;
	// Investment fail
	UPROPERTY(BlueprintReadOnly)
	FString RewardMessage;
};
