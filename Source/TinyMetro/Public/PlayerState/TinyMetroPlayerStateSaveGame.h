// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerItem.h"
#include "TinyMetroPlayerStateSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTinyMetroPlayerStateSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	int32 Money = 0;
	UPROPERTY()
	FPlayerItem ValidItem;
	UPROPERTY()
	FPlayerItem UsingItem;

	// Tutorial finish flags
	UPROPERTY()
	bool IsItemTutorialFinished;
	UPROPERTY()
	bool IsLaneTutorialFinished;
	UPROPERTY()
	bool IsStationTutorialFinished;
	UPROPERTY()
	bool IsTrainTutorialFinished;
	UPROPERTY()
	bool IsEventTutorialFinished;
	UPROPERTY()
	bool IsShopTutorialFinished;
	UPROPERTY()
	bool IsLoanTutorialFinished;
	UPROPERTY()
	bool IsInvestmentTutorialFinished;
	UPROPERTY()
	bool IsPolicyTutorialFinished;
};
