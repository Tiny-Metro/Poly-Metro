// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LanePoint.h"
#include "LaneManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ULaneManagerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FLanePoint> Lanes;


	UPROPERTY()
	bool CanAssignBridge;

	UPROPERTY()
	bool CanAssignTunnel;

	UPROPERTY()
	TArray<int32> NextLaneNums;


};
