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

	// Passenger effect
	UFUNCTION()
	FLuaValue GetArrivePassengerCount() const;
	UFUNCTION()
	FLuaValue GetNotArrivePassengerCount() const;
	UFUNCTION()
	FLuaValue GetNotArrivePassengerCountByType(FLuaValue Type) const;
	UFUNCTION()
	void AddPassengerSpawn(FLuaValue Rate) const;

	// Lane effect
	UFUNCTION()
	FLuaValue GetLaneCount() const;
	UFUNCTION()
	FLuaValue GetRandomLaneId() const;

	// Train effect
	UFUNCTION()
	FLuaValue GetTrainCount() const;
	UFUNCTION()
	FLuaValue GetMainTrainCount() const;
	UFUNCTION()
	FLuaValue GetSubTrainCount() const;
	UFUNCTION()
	FLuaValue GetTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetMainTrainCountByLane(FLuaValue Id) const;
	UFUNCTION()
	FLuaValue GetSubTrainCountByLane(FLuaValue Id) const;

protected:
	UPROPERTY()
	mutable class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	mutable class ATinyMetroPlayerState* PlayerStateRef;
	UPROPERTY()
	mutable class APolicy* PolicyRef;
	UPROPERTY()
	mutable class AStationManager* StationManagerRef;
};
