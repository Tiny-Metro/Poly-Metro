// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "InvestmentStartData.h"
#include "InvestmentManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UInvestmentManagerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<int32, FInvestmentStartData> InvestmentStartData;
	UPROPERTY()
	bool CanRefresh = true;
	UPROPERTY()
	int32 MaxInvestmentSlot = 3;
	UPROPERTY()
	int32 InvestmentSlot = 0;
	UPROPERTY()
	TMap<int32, int32> InvestmentCandidate;
};
