// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LuaState.h"
#include "InvestmentLuaState.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UInvestmentLuaState : public ULuaState
{
	GENERATED_BODY()
public:
	UInvestmentLuaState();
	static UInvestmentLuaState* CreateInstance(UWorld* WorldContextObject);

	// Getter need to wrapping LuaValue

	// PlayerState effect
	UFUNCTION()
	void AddMoney(FLuaValue Money);
	UFUNCTION()
	void AddLane(FLuaValue Num);
	UFUNCTION()
	void AddTrain(FLuaValue Num);
	UFUNCTION()
	void AddSubtrain(FLuaValue Num);
	UFUNCTION()
	void AddBridge(FLuaValue Num);
	UFUNCTION()
	void AddTunnel(FLuaValue Num);
	/*
	- GetBuiltBirdgeCount
	- GetBuiltTunnelCount
	- GetBuiltLaneCount
	- GetReduceLaneCount
	- GetRemoveLaneCount
	- GetArrivePassengerCount
	- GetBuyItemCount
	*/
	

	// Policy effect
	UFUNCTION()
	FLuaValue GetServiceLevel() const;
	UFUNCTION()
	FLuaValue GetPrioritySeat() const;
	UFUNCTION()
	FLuaValue GetCCTV() const;
	UFUNCTION()
	FLuaValue GetElevator() const;
	UFUNCTION()
	FLuaValue GetBicycle() const;
	UFUNCTION()
	FLuaValue GetTransfer() const;
	UFUNCTION()
	void SetServiceLevel(FLuaValue Level);
	UFUNCTION()
	void SetPrioritySeat(FLuaValue Flag);
	UFUNCTION()
	void SetCCTV(FLuaValue Flag);
	UFUNCTION()
	void SetElevator(FLuaValue Flag);
	UFUNCTION()
	void SetBicycle(FLuaValue Flag);
	UFUNCTION()
	void SetTransfer(FLuaValue Flag);

	// Station effect
	UFUNCTION()
	FLuaValue GetStationCount() const;
	UFUNCTION()
	FLuaValue GetActiveStationCount() const;
	UFUNCTION()
	FLuaValue GetInactiveStationCount() const;
	UFUNCTION()
	FLuaValue GetDestroyedStationCount() const;
	UFUNCTION()
	FLuaValue GetTypeStationCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetRandomStationType() const;
	UFUNCTION()
	FLuaValue GetRandomStationId() const;
	UFUNCTION()
	FLuaValue GetRandomActiveStationId() const;
	UFUNCTION()
	FLuaValue GetRandomInactiveStationId() const;
	UFUNCTION()
	FLuaValue GetRandomDestroyedStationId() const;
	UFUNCTION()
	FLuaValue GetRandomTypeStationId(FLuaValue Type) const;
	UFUNCTION()
	void AddComplainFixedValue(FLuaValue StationId, FLuaValue Complain);
	UFUNCTION()
	void AddComplainByRate(FLuaValue StationId, FLuaValue Rate);
	// GetStationCountByLane
	// GetMaxComplain
	// GetUpgradeStationCount
	// GetMaxTransferCount

	// Passenger effect
	UFUNCTION()
	FLuaValue GetTotalPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetFreePassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetPaidPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetArriveTotalPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetArriveFreePassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetArrivePaidPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetWaitTotalPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetWaitFreePassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetWaitPaidPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetDestroyedTotalPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetDestroyedFreePassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetDestroyedPaidPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetRideTotalPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetRideFreePassengerCount(FLuaValue Type) const;
	UFUNCTION()
	FLuaValue GetRidePaidPassengerCount(FLuaValue Type) const;
	UFUNCTION()
	void AddPassengerSpawn(FLuaValue Rate) const;

	// Lane effect
	UFUNCTION()
	FLuaValue GetLaneCount() const;
	UFUNCTION()
	FLuaValue GetRandomLaneId() const;
	// GetCircularLaneCount

	// Train effect
	UFUNCTION()
	FLuaValue GetTrainCount() const;
	UFUNCTION()
	FLuaValue GetUpgradeTrainCount() const;
	UFUNCTION()
	FLuaValue GetMainTrainCount() const;
	UFUNCTION()
	FLuaValue GetUpgradeMainTrainCount() const;
	UFUNCTION()
	FLuaValue GetSubTrainCount() const;
	UFUNCTION()
	FLuaValue GetUpgradeSubTrainCount() const;
	UFUNCTION()
	FLuaValue GetTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetUpgradeTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetMainTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetUpgradeMainTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetSubTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetUpgradeSubTrainCountByLane(FLuaValue Id) const;
	// GetMaxSubtrainCount
	
	// Bank
	// GetLoanAmount

protected:
	UPROPERTY()
	mutable class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	mutable class ATinyMetroPlayerState* PlayerStateRef;
	UPROPERTY()
	mutable class APolicy* PolicyRef;
	UPROPERTY()
	mutable class AStationManager* StationManagerRef;
	UPROPERTY()
	mutable class ALaneManager* LaneManagerRef;
	UPROPERTY()
	mutable class ATrainManager* TrainManagerRef;
};
