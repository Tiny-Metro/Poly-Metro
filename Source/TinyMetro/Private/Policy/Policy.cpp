// Fill out your copyright notice in the Description page of Project Settings.


#include "Policy/Policy.h"
#include "Policy/PolicySaveGame.h"
#include "Station/StationManager.h"
#include "SaveSystem/TMSaveManager.h"
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

	auto GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if( !IsValid(StationManagerRef)) StationManagerRef = GameModeRef->GetStationManager();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	

	InitPolicy();
	
	SaveManagerRef->SaveTask.AddDynamic(this, &APolicy::Save);
}

// Called every frame
void APolicy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APolicy::Save() {
	UPolicySaveGame* tmp = Cast<UPolicySaveGame>(UGameplayStatics::CreateSaveGameObject(UPolicySaveGame::StaticClass()));

	tmp->PolicyData = PolicyData;

	SaveManagerRef->Save(tmp, SaveActorType::Policy);
}

void APolicy::Load() {
	UPolicySaveGame* tmp = Cast<UPolicySaveGame>(SaveManagerRef->Load(SaveActorType::Policy));

	if (!IsValid(tmp)) return;

	PolicyData = tmp->PolicyData;
}

void APolicy::SetServiceCostLevel(int costLevel) {
	PolicyData.ServiceCostLevel = costLevel;
	PolicyUpdateTask.Broadcast();
}

void APolicy::SetPrioritySeat(bool Flag) {
	PolicyData.PrioritySeat = Flag;
	PolicyUpdateTask.Broadcast();
}

void APolicy::SetCCTV(bool Flag) {
	PolicyData.HasCCTV = Flag;
	PolicyUpdateTask.Broadcast();
}

void APolicy::SetElevator(bool Flag) {
	PolicyData.HasElevator = Flag;
	PolicyUpdateTask.Broadcast();
}

void APolicy::SetBicycle(bool Flag) {
	PolicyData.HasBicycle = Flag;
	PolicyUpdateTask.Broadcast();
}

void APolicy::SetTransfer(bool Flag) {
	PolicyData.HasTransfer = Flag;
	PolicyUpdateTask.Broadcast();
}

FPolicyData APolicy::GetPolicyData() const {
	return PolicyData;
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