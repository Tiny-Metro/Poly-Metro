// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DefaultStatistics.h"
#include "LaneStatistics.h"
#include "ShopStatistics.h"
#include "BankStatistics.h"
#include "StatisticsManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UStatisticsManagerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FDefaultStatistics DefaultStatistics;
	UPROPERTY(BlueprintReadWrite)
	FLaneStatistics LaneStatistics;
	UPROPERTY(BlueprintReadWrite)
	FShopStatistics ShopStatistics;
	UPROPERTY(BlueprintReadWrite)
	FBankStatistics BankStatistics;
};
