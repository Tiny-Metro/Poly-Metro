// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/GamePlayInfo.h"
#include "../Shop/ItemType.h"
#include "TinyMetroPlayerState.generated.h"

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
	ATinyMetroPlayerState();
	bool BuyItem(ItemType Type, int32 Cost, int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	int32 GetSales() const;
	UFUNCTION(BlueprintCallable)
	int32 GetProfit() const;
	UFUNCTION(BlueprintCallable)
	int32& GetMoney();

	virtual void Tick(float DeltaTime) override;

public:
	void AddMoney(int32 Amount);
	void AddSales(int32 Sale);
	void Test();

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
	int32 ValidSubTrain = 0;
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

	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStationManager* StationManager;
};
