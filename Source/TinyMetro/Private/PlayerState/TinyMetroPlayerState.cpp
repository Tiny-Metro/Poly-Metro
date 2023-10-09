// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/TinyMetroPlayerState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationManager.h"
#include "Timer/Timer.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManager.h"
#include <Kismet/GameplayStatics.h>

FGamePlayInfo ATinyMetroPlayerState::GetPlayInfo() {

	FGamePlayInfo info;
	info.PlayTime = 0; // TODO
	info.Sales = Sales;
	info.Profit = Profit;
	info.SalesInWeek = SalesInWeek;
	info.ProfitInWeek = ProfitInWeek;
	info.ComplainAverage = StationManager->GetComplainAverage();
	info.Passenger = Arrive;
	info.UsingTrain = UsingTrain;
	info.UsingLane = UsingLane;
	info.UsingTunnel = UsingTunnel;
	info.ConnectedStation = 0; // TODO

	return info;
}

void ATinyMetroPlayerState::BeginPlay() {
	Super::BeginPlay();

	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(StationManager)) StationManager = GameModeRef->GetStationManager();
	if (!IsValid(Timer)) Timer = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(1.0);

}

bool ATinyMetroPlayerState::BuyItem(ItemType Type, int32 Cost, int32 Amount) {
	if (Money >= Cost) {
		switch (Type) {
		case ItemType::Train:
			ValidTrain += Amount;
			break;
		case ItemType::Subtrain:
			ValidSubtrain += Amount;
			break;
		case ItemType::Lane:
			ValidLane += Amount;
			break;
		case ItemType::Tunnel:
			ValidTunnel += Amount;
			break;
		case ItemType::Bridge:
			ValidBridge += Amount;
			break;
		default:
			// TODO : Upgrade items
			break;
		}

		Money -= Cost * Amount;
		StatisticsManagerRef->DefaultStatistics.TotalSpending -= Cost * Amount;
		StatisticsManagerRef->DefaultStatistics.WeeklySpending -= Cost * Amount;
		Profit -= Cost * Amount;

		return true;
	} else {
		return false;
	}
}

bool ATinyMetroPlayerState::CanUseMoney(int32 Val) const {
	return this->Money >= Val ? true : false;
}

int32 ATinyMetroPlayerState::GetSales() const {
	return Sales;
}

int32 ATinyMetroPlayerState::GetProfit() const {
	return Profit;
}

int32 ATinyMetroPlayerState::GetValidBridgeCount() 
{
	return ValidBridge;
}
int32 ATinyMetroPlayerState::GetValidTunnelCount() 
{
	return ValidTunnel;
}
int32 ATinyMetroPlayerState::GetMoney() {
	return Money;
}

void ATinyMetroPlayerState::Tick(float DeltaTime) {
}

void ATinyMetroPlayerState::AddMoney(int32 Amount) {
	Money += Amount;
	
}

void ATinyMetroPlayerState::AddIncome(int32 Amount) {
	Money += Amount;
	StatisticsManagerRef->DefaultStatistics.TotalIncome += Amount;
	StatisticsManagerRef->DefaultStatistics.WeeklyIncome += Amount;
}

void ATinyMetroPlayerState::AddSales(int32 Sale) {
	Sales += Sale;
}

void ATinyMetroPlayerState::Test() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("PlayerState")));
	}
  
}

void ATinyMetroPlayerState::AddItem(ItemType Item, int Amount) {
	switch (Item) {
	case ItemType::Lane:
		ValidLane += Amount;
		break;
	case ItemType::Train:
		ValidTrain += Amount;
		break;
	case ItemType::Subtrain:
		ValidSubtrain += Amount;
		break;
	case ItemType::Bridge:
		ValidBridge += Amount;
		break;
	case ItemType::Tunnel:
		ValidTunnel += Amount;
		break;

	}
}

bool ATinyMetroPlayerState::UseLane() {
	if (ValidLane <= 0) {
		return false;
	} else {
		ValidLane--;
		UsingLane++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseTrain() {
	if (ValidTrain <= 0) {
		return false;
	} else {
		ValidTrain--;
		UsingTrain++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseSubtrain() {
	if (ValidSubtrain <= 0) {
		return false;
	} else {
		ValidSubtrain--;
		UsingSubTrain++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseBridge() {
	if (ValidBridge <= 0) {
		return false;
	} else {
		ValidBridge--;
		UsingBridge++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseTunnel() {
	if (ValidTunnel <= 0) {
		return false;
	} else {
		ValidTunnel--;
		UsingTunnel++;
		return true;
	}
}

void ATinyMetroPlayerState::Save() {
}

void ATinyMetroPlayerState::Load() {
}
