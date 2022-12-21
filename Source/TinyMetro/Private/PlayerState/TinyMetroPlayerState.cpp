// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/TinyMetroPlayerState.h"
#include "Station/StationManager.h"
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

ATinyMetroPlayerState::ATinyMetroPlayerState() {
	// Get StationManager
	StationManager = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));

}

bool ATinyMetroPlayerState::BuyItem(ItemType Type, int32 Cost, int32 Amount) {
	if (Money >= Cost) {
		switch (Type) {
		case ItemType::Train:
			ValidTrain += Amount;
			break;
		case ItemType::Subtrain:
			ValidSubTrain += Amount;
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
		Profit -= Cost * Amount;

		return true;
	} else {
		return false;
	}

}
