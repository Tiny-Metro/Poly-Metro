// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lua/TinyMetroLuaState.h"
#include "InvestmentLuaState.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UInvestmentLuaState : public UTinyMetroLuaState
{
	GENERATED_BODY()

public:
	UInvestmentLuaState();
	static UInvestmentLuaState* CreateInstance(UWorld* WorldContextObject);

	UFUNCTION()
	void InvestmentLuaStateTest(FLuaValue Val);
	UFUNCTION()
	void AddIncome(FLuaValue Money);
	UFUNCTION()
	void AddMoney(FLuaValue Money);
	UFUNCTION()
	void AddItem(FLuaValue Item, FLuaValue Amount);

	UFUNCTION()
	FLuaValue GetTimestampAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetDefaultStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetLaneStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	TArray<FLuaValue> GetLaneDetailStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetShopStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetTrainStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetSubtrainStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetBridgeStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetTunnelStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	FLuaValue GetBankStatisticsAtStart(FLuaValue Id);
	UFUNCTION()
	TArray<FLuaValue> GetStationInfosAtStart(FLuaValue Id);
	UFUNCTION()
	TArray<FLuaValue> GetTrainInfosAtStart(FLuaValue Id);
};
