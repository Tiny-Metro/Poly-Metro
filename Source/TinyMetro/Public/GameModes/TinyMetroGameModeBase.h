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
	virtual FString GetMapName() const;
	virtual TMap<FIntPoint, StationType> GetInitData() const;
	virtual int32 GetDaytime() const;
	UFUNCTION(BlueprintCallable)
	virtual TMap<int32, FColor> GetLaneColor() const;
	virtual TArray<FSoftObjectPath> GetTrainMaterialPath() const;
	virtual TArray<FSoftObjectPath> GetLaneMaterialPath() const;
	virtual TArray<FSoftObjectPath> GetPassengerMaterialPath() const;

	virtual void StartPlay() override;
	virtual void BeginPlay() override;
	void LoadData();

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
	UFUNCTION(BlueprintCallable)
	class ABridgeTunnelManager* GetBridgeTunnelManager() const;
	UFUNCTION(BlueprintCallable)
	class ATinyMetroEventManager* GetEventManager() const;
	UFUNCTION(BlueprintCallable)
	class AStatisticsManager* GetStatisticsManager() const;
	UFUNCTION(BlueprintCallable)
	class AInvestmentManager* GetInvestmentManager() const;
	UFUNCTION(BlueprintCallable)
	class ASoundManager* GetSoundManager() const;
	

	// Test
	UFUNCTION(BlueprintCallable)
	void TestFunction();

protected:
	UPROPERTY(Config)
	int32 TestValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Daytime;

	// Defined by DefaultGame.ini
	UPROPERTY(Config, VisibleAnywhere)
	TArray<FSoftObjectPath> TrainMaterialPath;
	UPROPERTY(Config, VisibleAnywhere)
	TArray<FSoftObjectPath> LaneMaterialPath;
	UPROPERTY(Config, VisibleAnywhere)
	TArray<FSoftObjectPath> PassengerMaterialPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AStationManager* StationManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AStationManager> StationManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ASoundManager* SoundManagerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASoundManager> SoundManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATrainManager* TrainManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ATrainManager> TrainManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ALaneManager* LaneManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ALaneManager> LaneManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABridgeTunnelManager* BridgeTunnelManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ABridgeTunnelManager> BridgeTunnelManagerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATMSaveManager* SaveManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APolicy* Policy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABank* Bank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AInvestmentManager* InvestmentManagerRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATimer* Timer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AShop* Shop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APlayerController* PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATinyMetroEventManager* EventManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AStatisticsManager* StatisticsManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	APawn* PawnRef;


};
