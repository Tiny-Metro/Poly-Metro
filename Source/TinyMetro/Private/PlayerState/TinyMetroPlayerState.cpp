// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/TinyMetroPlayerState.h"
#include "GameModes/TinyMetroGameModeBase.h"
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

void ATinyMetroPlayerState::BeginPlay() {
	TinyMetroGameModeBase = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	DayTime = TinyMetroGameModeBase->GetDaytime();

	Timer = GetWorld()->SpawnActor<ATimer>();
	TMSaveManager = GetWorld()->SpawnActor<ATMSaveManager>();
	/*TMSaveManager = GetWorld()->SpawnActorDeferred<ATMSaveManager>(ATMSaveManager::StaticClass(),
		FTransform());
	TMSaveManager->FinishSpawning(FTransform());*/
	/*
	if (IsValid(TinyMetroGameModeBase)) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White,
			FString::Printf(TEXT("Hi %d"), DayTime));
	}*/

	
	//GetDay();
	//GetDayTime();
	
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

int32 ATinyMetroPlayerState::GetSales() const {
	return Sales;
}

int32 ATinyMetroPlayerState::GetProfit() const {
	return Profit;
}

float ATinyMetroPlayerState::GetPlayTimeSec() {

	return Timer->ElapseTimeSec;
}

void ATinyMetroPlayerState::SetPlayTimeSec(float elapseTimeSec) {

	if (Timer == nullptr || Timer == NULL) {
		UE_LOG(LogTemp, Error, TEXT("Timer is nullptr!"));
		return;
	}
	else {
		Timer->ElapseTimeSec = elapseTimeSec;
	}

}

int32 ATinyMetroPlayerState::GetDay() {

	if (TinyMetroGameModeBase == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("TinyMetroGameModeBase is nullptr!"));
	}
	float CurrentSec= GetPlayTimeSec();

	int32 CurrentDay = CurrentSec / DayTime;
	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White,
		FString::Printf(TEXT("CurrentDay %d"), CurrentDay));*/

	return CurrentDay;

}


float ATinyMetroPlayerState::GetDayTime() {

	float CurrentHour = GetPlayTimeSec() - (GetDay() * DayTime);

	/*
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::White,
		FString::Printf(TEXT("CurrentHour %f"),CurrentHour));*/

	return CurrentHour;
int32& ATinyMetroPlayerState::GetMoney() {
	return Money;
}

void ATinyMetroPlayerState::Tick(float DeltaTime) {
	// Test code : Need to Remove
	Money++;
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Money : %d"), Money));
	}*/
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