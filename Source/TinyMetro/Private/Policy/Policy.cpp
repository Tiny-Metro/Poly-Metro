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

	//stationmanager = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));
	stationmanager = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();

	PolicyData.ServiceCostLevel = 3;
	PolicyData.PrioritySeat = false;
	PolicyData.HasCCTV = false;
	PolicyData.HasElevator = false;
	PolicyData.HasBicycle = false;
	PolicyData.HasTransfer = false;


	InitPolicy();
	
}

// Called every frame
void APolicy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APolicy::SetServiceCostLevel(int costLevel) {
	PolicyData.ServiceCostLevel = costLevel;

	//TODO ServiceCostlevel 함수 추가
}

void APolicy::SetPrioritySeat(bool Flag) {
	bool pre = PolicyData.PrioritySeat;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO 핸디캡 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO 핸디캡 함수 추가
	}

	PolicyData.PrioritySeat = !pre;

	PolicyData.PrioritySeat = Flag;
}

void APolicy::SetCCTV(bool Flag) {
	bool pre = PolicyData.HasCCTV;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO cctv 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO cctv 함수 추가
	}

	PolicyData.HasCCTV = !pre;

	PolicyData.HasCCTV = Flag;
}

void APolicy::SetElevator(bool Flag) {
	bool pre = PolicyData.HasElevator;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO elevator 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO elevator 함수 추가
	}

	PolicyData.HasElevator = !pre;

	PolicyData.HasElevator = Flag;
}

void APolicy::SetBicycle(bool Flag) {
	bool pre = PolicyData.HasBicycle;

	if (!pre) { // 전의 상태가 비활성화면 -> 활성화되면

		//승객 수 10% 증가

		for (int i = 0; i < stationmanager->Station.Num(); i++)
		{
			stationmanager->Station[0]->AddPassengerSpawnProbability(0.1, -1);
		}
	}
	else { // 전의 상태가 활성화
		
		for (int i = 0; i < stationmanager->Station.Num(); i++)
		{
			stationmanager->Station[0]->AddPassengerSpawnProbability(-0.1, -1);
		}
	}

	PolicyData.HasBicycle = !pre;

	PolicyData.HasBicycle = Flag;
}

void APolicy::SetTransfer(bool Flag) {
	bool pre = PolicyData.HasTransfer;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO transfer 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO transfer 함수 추가
	}

	PolicyData.HasTransfer = !pre;

	PolicyData.HasTransfer = Flag;
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

	int ComplainAmount = ComplainArrayForServiceLevel[GetServiceCostLevel()];

	return ComplainAmount;
}

int APolicy::GetCostForServiceLevel() {
	int Cost = CostArrayForServiceLevel[GetServiceCostLevel()];

	return Cost;
}

int APolicy::GetCostForCCTV() {
	if (GetCCTV()) {
		return 5;
	}
	else {
		return 0;
	}
}

int APolicy::GetCostForElevator() {
	if (GetElevator()) {
		return 10;
	}
	else {
		return 0;
	}
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
}