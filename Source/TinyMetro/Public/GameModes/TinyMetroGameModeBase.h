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
UCLASS(Config = Game)
class TINYMETRO_API ATinyMetroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATinyMetroGameModeBase();
	virtual FString GetFileName() const;
	virtual TMap<FIntPoint, StationType> GetInitData() const;
	virtual int32 GetDaytime() const;
	virtual TArray<FStringAssetReference> GetTrainMaterialPath() const;

	virtual void StartPlay() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetGameSpeed(float TimeDilation);

protected:
	UPROPERTY(Config)
	int32 TestValue;
	UPROPERTY(Config)
	int32 Daytime;
	UPROPERTY(Config)
	TArray<FStringAssetReference> TrainMaterialPath;
};
