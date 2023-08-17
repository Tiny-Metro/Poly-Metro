// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentState.h"
#include "InvestmentData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FInvestmentData
{
	GENERATED_USTRUCT_BODY()

public:
	// Investment id
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Id;
	// Mission require text
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Message;
	// Require day
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TimeRequire;
	// Award text
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Award;
	// Investment State { Ready, Processing, Success, Fail }
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	InvestmentState State = InvestmentState::Ready;
};
