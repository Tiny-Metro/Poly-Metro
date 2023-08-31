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

};
