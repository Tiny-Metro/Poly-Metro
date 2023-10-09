// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class SaveActorType : uint8
{
	StationManager = 0 UMETA(DisplayName = "StationMananger"),
	Station = 1 UMETA(DisplayName = "Station"),
	LaneManager = 2 UMETA(DisplayName = "LaneManager"),
	Lane = 3 UMETA(DisplayName = "Lane"),
	TrainManager = 4 UMETA(DisplayName = "TrainManager"),
	Train = 5 UMETA(DisplayName = "Train"),
	StatisticsManager = 6 UMETA(DisplayName = "StatisticsManager"),
	BridgeTunnelManager = 7 UMETA(DisplayName = "BridgeTunnelManager"),
	BridgeTunnel = 8 UMETA(DisplayName = "BridgeTunnel"),
	Policy = 9 UMETA(DisplayName = "Policy"),
	Camera = 10 UMETA(DisplayName = "Camera"),
	EventManager = 10 UMETA(DisplayName = "EventManager"),
	InvestmentManager = 11 UMETA(DisplayName = "InvestmentManager"),
	Investment = 12 UMETA(DisplayName = "Investment"),
	Timer = 13 UMETA(DisplayName = "Timer"),
	Bank = 14 UMETA(DisplayName = "Bank"),
	Loan = 15 UMETA(DisplayName = "Loan"),
	PlayerState = 16 UMETA(DisplayName = "PlayerState")
	
};
