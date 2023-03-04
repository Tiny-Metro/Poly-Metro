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
	virtual TArray<FSoftObjectPath> GetTrainMaterialPath() const;
	virtual TArray<FSoftObjectPath> GetLaneMaterialPath() const;

	virtual void StartPlay() override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetGameSpeed(float TimeDilation);
	UFUNCTION(BlueprintCallable)
	class AStationManager* GetStationManager() const;
	UFUNCTION(BlueprintCallable)
	class ATMSaveManager* GetSaveManager() const;
	UFUNCTION(BlueprintCallable)
	class ATrainManager* GetTrainManager() const;
	UFUNCTION(BlueprintCallable)
	class ALaneManager* GetLaneManager() const;
	UFUNCTION(BlueprintCallable)
	class APolicy* GetPolicy() const;
	UFUNCTION(BlueprintCallable)
	class ABank* GetBank() const;
	UFUNCTION(BlueprintCallable)
	class ATimer* GetTimer() const;
	UFUNCTION(BlueprintCallable)
	class AShop* GetShop() const;

protected:
	UPROPERTY(Config)
	int32 TestValue;
	UPROPERTY(Config)
	int32 Daytime;
	UPROPERTY(Config)
	TArray<FSoftObjectPath> TrainMaterialPath;
	UPROPERTY(Config)
	TArray<FSoftObjectPath> LaneMaterialPath;
	UPROPERTY()
	class AStationManager* StationManager;
	UPROPERTY()
	class ATMSaveManager* SaveManager;
	UPROPERTY()
	class ATrainManager* TrainManager;
	UPROPERTY()
	class ALaneManager* LaneManager;
	UPROPERTY()
	class APolicy* Policy;
	UPROPERTY()
	class ABank* Bank;
	UPROPERTY()
	class ATimer* Timer;
	UPROPERTY()
	class AShop* Shop;

};
