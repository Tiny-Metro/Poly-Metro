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
	FLuaValue GetRandomStationId() const;
	UFUNCTION()
	void AddComplainFixedValue(FLuaValue StationId, FLuaValue Complain);
	UFUNCTION()
	void AddComplainByRate(FLuaValue StationId, FLuaValue Rate);

	// Passenger effect
	UFUNCTION()
	void AddPassengerSpawn(FLuaValue Rate) const;

	// Train effect
	UFUNCTION()
	FLuaValue GetNumOfAllTrain() const;
	UFUNCTION()
	FLuaValue GetNumOfTrain(FLuaValue LaneId) const;

	// Lane effect
	UFUNCTION()
	FLuaValue GetNumOfLane() const;

protected:
	UPROPERTY()
	mutable class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	mutable class ATinyMetroPlayerState* PlayerStateRef;
	UPROPERTY()
	mutable class APolicy* PolicyRef;
};
