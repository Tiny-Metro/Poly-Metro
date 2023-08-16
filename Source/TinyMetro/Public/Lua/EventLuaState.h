// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Lua/TinyMetroLuaState.h"
#include "EventLuaState.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UEventLuaState : public UTinyMetroLuaState
{
	GENERATED_BODY()

public:
	UEventLuaState();
	static UEventLuaState* CreateInstance(UWorld* WorldContextObject);

	UFUNCTION()
	void AddStation(FLuaValue Amount);
	UFUNCTION()
	void DestroyStation(FLuaValue Amount);
	UFUNCTION()
	void AddPassengerSpawnProbability(FLuaValue Amount);
	UFUNCTION()
	void AddPassengerSpawnProbabilityByType(FLuaValue Type, FLuaValue Amount);
	UFUNCTION()
	void ScaleComplain(FLuaValue ScaleFactor);
	UFUNCTION()
	void AddComplainIncreaseRate(FLuaValue Rate);
};
