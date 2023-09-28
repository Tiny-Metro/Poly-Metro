// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TinyMetroEventOccurData.h"
#include "TinyMetroEventManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTinyMetroEventManagerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	uint32 ProgressDay = 0;
	UPROPERTY()
	int32 ExpirationDay = 0;
	UPROPERTY()
	TArray<FTinyMetroEventOccurData> EventOccurLog;
};
