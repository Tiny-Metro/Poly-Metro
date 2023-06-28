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
	FInvestmentData(FString Title, int32 TimeLimit, FString AwardMessage) :
		Title(Title),
		TimeLimit(TimeLimit),
		AwardMessage(AwardMessage){ }

	// Investmet title
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Title;
	// Require day
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TimeLimit;
	// Investment success
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString AwardMessage;
};
