// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Timestamp.h"
#include "TimerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTimerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float ElapseTimeSec;
	UPROPERTY()
	FTimestamp Timestamp;
	UPROPERTY()
	float DayCounter = 0.0f;
	UPROPERTY()
	float WeekCounter = 0.0f;
	UPROPERTY()
	float SkiptimeTarget = 0.0f;
	UPROPERTY()
	bool SkiptimeFlag = false;
};
