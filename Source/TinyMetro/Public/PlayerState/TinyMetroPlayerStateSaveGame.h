// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
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
};
