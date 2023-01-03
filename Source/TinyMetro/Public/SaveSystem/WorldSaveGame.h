// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WorldSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UWorldSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
		float ElapseTime;
	
};
