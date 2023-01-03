// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "../Timer/Timer.h"
#include "GameModeBaseSeoul.generated.h"


/**
 *
 */
UCLASS()
class TINYMETRO_API AGameModeBaseSeoul : public ATinyMetroGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeBaseSeoul();
	virtual FString GetFileName() const override;
	virtual TMap<FIntPoint, StationType> GetInitData() const override;
	virtual int32 GetDaytime() const override;

	virtual void BeginPlay() override;

	UFUNCTION()
		float GetTime();

	TSubclassOf<ATimer> ActorToSpawn;

	ATimer* Timer;
};
