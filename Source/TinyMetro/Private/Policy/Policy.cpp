// Fill out your copyright notice in the Description page of Project Settings.


#include "Policy/Policy.h"
#include "Station/StationManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APolicy::APolicy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APolicy::BeginPlay()
{
	Super::BeginPlay();

	StationManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();

	InitPolicy();
	
}

// Called every frame
void APolicy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APolicy::SetServiceCostLevel(int costLevel) {
	PolicyData.ServiceCostLevel = costLevel;

	StationManagerRef->SetServiceData(ServiceLevel[costLevel]);
}

void APolicy::SetPrioritySeat(bool Flag) {
	PolicyData.PrioritySeat = Flag;
	StationManagerRef->AddComplainIncreaseRate(-0.05 * (Flag ? 1 : -1), -1);
	// TODO : Imple in StationManager
	//StationManagerRef->AddFreePassengerSpawnProbability(0.2 * (Flag ? 1 : -1), -1);
}

void APolicy::SetCCTV(bool Flag) {
	PolicyData.HasCCTV = Flag;
	StationManagerRef->AddComplainIncreaseRate(-0.1 * (Flag ? 1 : -1), -1);
}

void APolicy::SetElevator(bool Flag) {
	PolicyData.HasElevator = Flag;
	StationManagerRef->AddComplainIncreaseRate(-0.15 * (Flag ? 1 : -1), -1);
}

void APolicy::SetBicycle(bool Flag) {
	PolicyData.HasBicycle = Flag;
	// TODO : Imple in StationManager
	//StationManagerRef->AddPassengerSpawnProbability(0.1 * (Flag ? 1 : -1), -1);
	StationManagerRef->AddComplainIncreaseRate(0.1 * (Flag ? 1 : -1), -1);
}

void APolicy::SetTransfer(bool Flag) {
	PolicyData.HasTransfer = Flag;
	StationManagerRef->AddComplainIncreaseRate(-0.2 * (Flag ? 1 : -1), -1);
	StationManagerRef->SetTransfer(Flag);
}

int APolicy::GetServiceCostLevel() {
	return PolicyData.ServiceCostLevel;
}

bool APolicy::GetPrioritySeat() {
	return PolicyData.PrioritySeat;
}

bool APolicy::GetCCTV() {
	return PolicyData.HasCCTV;
}

bool APolicy::GetElevator() {
	return PolicyData.HasElevator;
}

bool APolicy::GetBicycle() {
	return PolicyData.HasBicycle;
}

bool APolicy::GetTransfer() {
	return PolicyData.HasTransfer;
}

int APolicy::GetComplainForServiceLevel() {
	return ServiceLevel[GetServiceCostLevel()].DailyComplain;
}

int APolicy::GetCostForServiceLevel() {
	return ServiceLevel[GetServiceCostLevel()].WeeklyCost;
}

int APolicy::GetCostForCCTV() {
	return GetCCTV() ? 5 : 0;
}

int APolicy::GetCostForElevator() {
	return GetElevator() ? 10 : 0;
}

int APolicy::GetTotalCost() {
	return GetCostForServiceLevel() + 
		GetCostForCCTV() + 
		GetCostForElevator();
}


int32 APolicy::CalculateComplainPercentage() {

	int32 currentPercentage = 0; //음수면 감소량

	if (GetPrioritySeat()) {
		currentPercentage -= 5;
	}

	if (GetCCTV()) {
		currentPercentage -= 10;
	}

	if (GetElevator()) {
		currentPercentage -= 15;
	}

	if (GetBicycle()) {
		currentPercentage += 10;
	}

	if (GetTransfer()) {
		currentPercentage -= 20;
	}

	return currentPercentage;
}


void APolicy::InitPolicy() {
	PolicyData.ServiceCostLevel = 3;
	PolicyData.PrioritySeat = false;
	PolicyData.HasCCTV = false;
	PolicyData.HasElevator = false;
	PolicyData.HasBicycle = false;
	PolicyData.HasTransfer = false;
}