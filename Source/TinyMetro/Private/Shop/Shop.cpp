// Fill out your copyright notice in the Description page of Project Settings.


#include "Shop/Shop.h"
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

	//PlayerState = GetWorld()->GetPlayerControllerIterator()->Get()->GetPlayerState<ATinyMetroPlayerState>();
	PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (IsValid(PlayerState)) {
		UE_LOG(LogTemp, Log, TEXT("Shop::BeginPlay : PlayerState Valid"));
	}else{
		UE_LOG(LogTemp, Log, TEXT("Shop::BeginPlay : PlayerState Invalid"));
	}

	UpdateShopInfo();
	
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShop::UpdateShopInfo() {
	ShopInfo.CostTrain += CostTrainIncrease * PurchaseNumTrain;
	ShopInfo.CostSubtrain += CostSubtrainIncrease * PurchaseNumSubtrain;

}

FShopInfo AShop::GetShopInfo() {
	UpdateShopInfo();
	return ShopInfo;
}

FShopInfo AShop::PurchaseTrain(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::Train, ShopInfo.CostTrain, Amount);
	if (Success) {
		PurchaseNumTrain += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseSubtrain(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::Subtrain, ShopInfo.CostSubtrain, Amount);
	if (Success) {
		PurchaseNumSubtrain += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseLane(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::Lane, ShopInfo.CostLane, Amount);
	if (Success) {
		PurchaseNumLane += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseTunnel(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::Tunnel, ShopInfo.CostTunnel, Amount);
	if (Success) {
		PurchaseNumTunnel += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseBridge(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::Bridge, ShopInfo.CostBridge, Amount);
	if (Success) {
		PurchaseNumBridge += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseUpgradeTrain(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::UpgradeTrain, ShopInfo.CostUpgradeTrain, Amount);
	if (Success) {
		PurchaseNumUpgradeTrain += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseUpgradeSubtrain(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::UpgradeSubtrain, ShopInfo.CostUpgradeSubtrain, Amount);
	if (Success) {
		PurchaseNumUpgradeSubtrain += Amount;
	}
	return GetShopInfo();
}

FShopInfo AShop::PurchaseUpgradeStation(int32 Amount, bool& Success) {
	Success = PlayerState->BuyItem(ItemType::UpgradeStation, ShopInfo.CostUpgradeStation, Amount);
	if (Success) {
		PurchaseNumUpgradeStation += Amount;
	}
	return GetShopInfo();
}

