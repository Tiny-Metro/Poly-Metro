// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LuaState.h"
#include "TinyMetroLuaState.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTinyMetroLuaState : public ULuaState
{
	GENERATED_BODY()

public:
	UTinyMetroLuaState();
	static UTinyMetroLuaState* CreateInstance(UWorld* WorldContextObject);
	
	UFUNCTION()
	void InitReferClasses();

	UFUNCTION()
	FLuaValue GetTimestamp();
	UFUNCTION()
	FLuaValue GetPolicyData();
	UFUNCTION()
	FLuaValue GetDefaultStatistics();
	UFUNCTION()
	FLuaValue GetLaneStatistics();
	UFUNCTION()
	TArray<FLuaValue> GetLaneDetailStatistics();
	UFUNCTION()
	FLuaValue GetShopStatistics();
	UFUNCTION()
	FLuaValue GetTrainStatistics();
	UFUNCTION()
	FLuaValue GetSubtrainStatistics();
	UFUNCTION()
	FLuaValue GetBridgeStatistics();
	UFUNCTION()
	FLuaValue GetTunnelStatistics();
	UFUNCTION()
	FLuaValue GetBankStatistics();
	UFUNCTION()
	TArray<FLuaValue> GetStationInfos();
	UFUNCTION()
	TArray<FLuaValue> GetTrainInfos();

protected:
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	class ATinyMetroPlayerState* PlayerStateRef;
	UPROPERTY()
	class AGridManager* GridManagerRef;
	UPROPERTY()
	class ATimer* TimerRef;
	UPROPERTY()
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY()
	class AStationManager* StationManagerRef;
	UPROPERTY()
	class ATrainManager* TrainManagerRef;
	UPROPERTY()
	class APolicy* PolicyRef;
};
