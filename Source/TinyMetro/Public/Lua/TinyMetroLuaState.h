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
	FLuaValue GetDefaultStatistics();
	UFUNCTION()
	FLuaValue GetLaneStatistics();
	UFUNCTION()
	FLuaValue GetShopStatistics();
	UFUNCTION()
	FLuaValue GetBankStatistics();

protected:
	UPROPERTY()
	mutable class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	mutable class AStatisticsManager* StatisticsManagerRef;
};
