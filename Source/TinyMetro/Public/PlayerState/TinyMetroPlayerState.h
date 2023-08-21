// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/GamePlayInfo.h"
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
	bool CanUseMoney(int32 Money) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetSales() const;
	UFUNCTION(BlueprintCallable)
	int32 GetProfit() const;
	UFUNCTION(BlueprintCallable)
	int32 GetMoney();

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Arrive = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Money = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Sales = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Profit = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 SalesInWeek = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 ProfitInWeek = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ValidLane = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ValidTrain = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ValidSubtrain = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ValidExpress = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ValidTunnel = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 ValidBridge = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 Transfer = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Resource")
	int32 Investment = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Using")
	int32 UsingLane = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Using")
	int32 UsingTrain = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Using")
	int32 UsingSubTrain = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Using")
	int32 UsingExpress = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Using")
	int32 UsingTunnel = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Using")
	int32 UsingBridge = 0;	

	UPROPERTY()
	int32 DayTime;

	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStationManager* StationManager;

	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY()
	class ATimer* Timer;

	UPROPERTY()
	class ATMSaveManager* TMSaveManager;

	UPROPERTY()
	class ATinyMetroGameModeBase* TinyMetroGameModeBase;
};
