// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/TinyMetroPlayerState.h"
#include "PlayerState/TinyMetroPlayerStateSaveGame.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationManager.h"
#include "Timer/Timer.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManager.h"
#include <Kismet/GameplayStatics.h>

FGamePlayInfo ATinyMetroPlayerState::GetPlayInfo() {

	FGamePlayInfo info;


	return info;
}

void ATinyMetroPlayerState::BeginPlay() {
	Super::BeginPlay();

	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(StationManager)) StationManager = GameModeRef->GetStationManager();
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();

	PrimaryActorTick.bCanEverTick = true;
	SetActorTickInterval(1.0);

	Load();

	SaveManagerRef->SaveTask.AddDynamic(this, &ATinyMetroPlayerState::Save);
}

bool ATinyMetroPlayerState::BuyItem(ItemType Type, int32 Cost, int32 Amount) {
	if (Money >= Cost) {
		switch (Type) {
		case ItemType::Train:
			ValidItem.Train += Amount;
			break;
		case ItemType::Subtrain:
			ValidItem.Subtrain += Amount;
			break;
		case ItemType::Lane:
			ValidItem.Lane += Amount;
			break;
		case ItemType::Tunnel:
			ValidItem.Tunnel += Amount;
			break;
		case ItemType::Bridge:
			ValidItem.Bridge += Amount;
			break;
		default:
			break;
		}

		Money -= Cost * Amount;
		StatisticsManagerRef->DefaultStatistics.TotalSpending -= Cost * Amount;
		StatisticsManagerRef->DefaultStatistics.WeeklySpending -= Cost * Amount;

		return true;
	} else {
		return false;
	}
}

bool ATinyMetroPlayerState::CanUseMoney(int32 Val) const {
	return this->Money >= Val ? true : false;
}

int32 ATinyMetroPlayerState::GetValidBridgeCount() {
	return ValidItem.Bridge;
}

int32 ATinyMetroPlayerState::GetValidTunnelCount() {
	return ValidItem.Tunnel;
}

int32 ATinyMetroPlayerState::GetMoney() {
	return Money;
}

FPlayerItem ATinyMetroPlayerState::GetValidItem() {
	return ValidItem;
}

FPlayerItem ATinyMetroPlayerState::GetUsingItem() {
	return UsingItem;
}

void ATinyMetroPlayerState::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ATinyMetroPlayerState::EndPlay(EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason);
	Save();
}

void ATinyMetroPlayerState::AddMoney(int32 Amount) {
	Money += Amount;
}

void ATinyMetroPlayerState::AddIncome(int32 Amount) {
	Money += Amount;
	StatisticsManagerRef->DefaultStatistics.TotalIncome += Amount;
	StatisticsManagerRef->DefaultStatistics.WeeklyIncome += Amount;
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
		ValidItem.Lane += Amount;
		break;
	case ItemType::Train:
		ValidItem.Train += Amount;
		break;
	case ItemType::Subtrain:
		ValidItem.Subtrain += Amount;
		break;
	case ItemType::Bridge:
		ValidItem.Bridge += Amount;
		break;
	case ItemType::Tunnel:
		ValidItem.Tunnel += Amount;
		break;
	}
}

bool ATinyMetroPlayerState::UseLane() {
	if (ValidItem.Lane <= 0) {
		return false;
	} else {
		ValidItem.Lane--;
		UsingItem.Lane++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseTrain() {
	if (ValidItem.Train <= 0) {
		return false;
	} else {
		ValidItem.Train--;
		UsingItem.Train++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseSubtrain() {
	if (ValidItem.Subtrain <= 0) {
		return false;
	} else {
		ValidItem.Subtrain--;
		UsingItem.Subtrain++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseBridge() {
	if (ValidItem.Bridge <= 0) {
		return false;
	} else {
		ValidItem.Bridge--;
		UsingItem.Bridge++;
		return true;
	}
}

bool ATinyMetroPlayerState::UseTunnel() {
	if (ValidItem.Tunnel <= 0) {
		return false;
	} else {
		ValidItem.Tunnel--;
		UsingItem.Tunnel++;
		return true;
	}
}

void ATinyMetroPlayerState::Save() {
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	UTinyMetroPlayerStateSaveGame* tmp = Cast<UTinyMetroPlayerStateSaveGame>(UGameplayStatics::CreateSaveGameObject(UTinyMetroPlayerStateSaveGame::StaticClass()));

	tmp->Money = Money;
	tmp->ValidItem = ValidItem;
	tmp->UsingItem = UsingItem;

	SaveManagerRef->Save(tmp, SaveActorType::PlayerState);
}

void ATinyMetroPlayerState::Load() {
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	UTinyMetroPlayerStateSaveGame* tmp = Cast<UTinyMetroPlayerStateSaveGame>(SaveManagerRef->Load(SaveActorType::PlayerState));

	if (!IsValid(tmp)) {
		return;
	}

	Money = tmp->Money;
	ValidItem = tmp->ValidItem;
	UsingItem = tmp->UsingItem;
}
