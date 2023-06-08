// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FInvestmentData
{
	GENERATED_BODY()
	FInvestmentData() = default;
	FInvestmentData(FString Title, FString Content, int32 TimeRequire, FString RequireText, FString AcceptMessage, FString AwardMessage, FString RewardMessage) :
		Title(Title),
		Content(Content),
		TimeRequire(TimeRequire),
		RequireText(RequireText),
		AcceptMessage(AcceptMessage),
		AwardMessage(AwardMessage),
		RewardMessage(RewardMessage){ }

	// Investmet title
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Title;
	// Flavor text
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Content;
	// Require day
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TimeRequire;
	// Require condition
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString RequireText;
	// Deposit of investment
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString AcceptMessage;
	// Investment success
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString AwardMessage;
	// Investment fail
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString RewardMessage;
};
