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
	FInvestmentData(int32 TimeRequire, int32 Award, int32 Reward, FString Message) :
		TimeRequire(TimeRequire),
		Award(Award),
		Reward(Reward),
		Message(Message) { }
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TimeRequire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Award;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Reward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;
};
