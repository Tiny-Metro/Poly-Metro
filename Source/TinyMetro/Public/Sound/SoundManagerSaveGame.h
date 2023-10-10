// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SoundManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API USoundManagerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	float MasterVolume;
	UPROPERTY()
	float BackgroundVolume;
	UPROPERTY()
	float EffectVolume;
	
};
