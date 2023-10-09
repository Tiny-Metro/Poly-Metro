// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/GamePlayInfo.h"
#include "PlayerItem.h"
#include "../Shop/ItemType.h"
#include "../Timer/Timer.h"
#include "TinyMetroPlayerState.generated.h"

class ATinyMetroGameModeBase;

/**
 * 
 */
UCLASS()
class TINYMETRO_API ATinyMetroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FGamePlayInfo GetPlayInfo();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Call by shop
	UFUNCTION(BlueprintCallable)
	bool BuyItem(ItemType Type, int32 Cost, int32 Amount);
	UFUNCTION(BlueprintCallable)
	bool CanUseMoney(int32 Val) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetMoney();
	UFUNCTION(BlueprintCallable)
	FPlayerItem GetValidItem();
	UFUNCTION(BlueprintCallable)
	FPlayerItem GetUsingItem();

	UFUNCTION()
	int32 GetValidBridgeCount();

	UFUNCTION()
	int32 GetValidTunnelCount();

public:
	UFUNCTION(BlueprintCallable)
	void AddMoney(int32 Amount);
	UFUNCTION(BlueprintCallable)
	void AddIncome(int32 Amount);
	UFUNCTION(BlueprintCallable)
	void AddSales(int32 Sale);
	void Test();

	UFUNCTION(BlueprintCallable)
	void AddItem(ItemType Item, int Amount);
	UFUNCTION(BlueprintCallable)
	bool UseLane();
	UFUNCTION(BlueprintCallable)
	bool UseTrain();
	UFUNCTION(BlueprintCallable)
	bool UseSubtrain();
	UFUNCTION(BlueprintCallable)
	bool UseBridge();
	UFUNCTION(BlueprintCallable)
	bool UseTunnel();

	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Money = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPlayerItem ValidItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPlayerItem UsingItem;

	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStationManager* StationManager;
	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY()
	class ATimer* TimerRef;
	UPROPERTY()
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
};
