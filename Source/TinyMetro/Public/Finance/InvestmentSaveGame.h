// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InvestmentState.h"
#include "InvestmentSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UInvestmentSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool IsStart;
	UPROPERTY()
	int32 RemainTime;
	UPROPERTY()
	InvestmentState State;
};
