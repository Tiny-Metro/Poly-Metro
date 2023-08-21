// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/Shop.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Statistics/StatisticsManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AShop::AShop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(PlayerState)) PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();

	UpdateShopInfo();
	
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::UpdateShopInfo() {
	ShopInfo.CostTrain += CostTrainIncrease * StatisticsManagerRef->ShopStatistics.TrainStatistics.TotalPurchaseCount;
	ShopInfo.CostSubtrain += CostSubtrainIncrease * StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalPurchaseCount;

}

FShopInfo AShop::GetShopInfo() {
	UpdateShopInfo();
	return ShopInfo;
}

FShopInfo AShop::PurchaseTrain(int32 Amount, bool& Success) {
	Success = PlayerState->CanUseMoney(ShopInfo.CostTrain * Amount);
	if (Success) {
		PlayerState->AddItem(ItemType::Train, Amount);

		StatisticsManagerRef->ShopStatistics.TotalUsingMoney += ShopInfo.CostTrain * Amount;
		StatisticsManagerRef->ShopStatistics.TotalPurchaseCount += Amount;
		StatisticsManagerRef->ShopStatistics.TrainStatistics.TotalPurchaseCount += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseSubtrain(int32 Amount, bool& Success) {
	Success = PlayerState->CanUseMoney(ShopInfo.CostSubtrain * Amount);
	if (Success) {
		PlayerState->AddItem(ItemType::Subtrain, Amount);

		StatisticsManagerRef->ShopStatistics.TotalUsingMoney += ShopInfo.CostSubtrain * Amount;
		StatisticsManagerRef->ShopStatistics.TotalPurchaseCount += Amount;
		StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalPurchaseCount += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseLane(int32 Amount, bool& Success) {
	Success = PlayerState->CanUseMoney(ShopInfo.CostLane * Amount);
	if (Success) {
		PlayerState->AddItem(ItemType::Lane, Amount);

		StatisticsManagerRef->ShopStatistics.TotalUsingMoney += ShopInfo.CostLane * Amount;
		StatisticsManagerRef->ShopStatistics.TotalPurchaseCount += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseTunnel(int32 Amount, bool& Success) {
	Success = PlayerState->CanUseMoney(ShopInfo.CostTunnel * Amount);
	if (Success) {
		PlayerState->AddItem(ItemType::Tunnel, Amount);

		StatisticsManagerRef->ShopStatistics.TotalUsingMoney += ShopInfo.CostTunnel * Amount;
		StatisticsManagerRef->ShopStatistics.TotalPurchaseCount += Amount;
		StatisticsManagerRef->ShopStatistics.TunnelStatistics.TotalPurchaseCount += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseBridge(int32 Amount, bool& Success) {
	Success = PlayerState->CanUseMoney(ShopInfo.CostBridge * Amount);
	if (Success) {
		PlayerState->AddItem(ItemType::Bridge, Amount);

		StatisticsManagerRef->ShopStatistics.TotalUsingMoney += ShopInfo.CostBridge * Amount;
		StatisticsManagerRef->ShopStatistics.TotalPurchaseCount += Amount;
		StatisticsManagerRef->ShopStatistics.BridgeStatistics.TotalPurchaseCount += Amount;
	}
	return GetShopInfo();
}
