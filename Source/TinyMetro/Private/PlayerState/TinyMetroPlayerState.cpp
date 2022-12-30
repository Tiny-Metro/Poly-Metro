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

int32 ATinyMetroPlayerState::GetSales() const {
	return Sales;
}

int32 ATinyMetroPlayerState::GetProfit() const {
	return Profit;
}

void ATinyMetroPlayerState::AddMoney(int32 Amount) {
	Money += Amount;
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
