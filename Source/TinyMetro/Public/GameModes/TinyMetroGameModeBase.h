// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Station/StationType.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "TinyMetroGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ATinyMetroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATinyMetroGameModeBase();
	virtual FString GetFileName() const;
	virtual TMap<FIntPoint, StationType> GetInitData() const;
	virtual int32 GetDaytime() const;
};
