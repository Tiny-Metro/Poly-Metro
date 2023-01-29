// Fill out your copyright notice in the Description page of Project Settings.


#include "Policy/Policy.h"
#include "Station/StationManager.h"
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

	stationmanager = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));

	PolicyData.ServiceCostLevel = 3;
	PolicyData.HandicappedSeat = false;
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

void APolicy::SetHandicappedSeat( ) {
	bool pre = PolicyData.HandicappedSeat;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO 핸디캡 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO 핸디캡 함수 추가
	}

	PolicyData.HandicappedSeat = !pre;

}

void APolicy::SetHasCCTV() {
	bool pre = PolicyData.HasCCTV;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO cctv 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO cctv 함수 추가
	}

	PolicyData.HasCCTV = !pre;
}

void APolicy::SetHasElevator( ) {
	bool pre = PolicyData.HasElevator;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO elevator 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO elevator 함수 추가
	}

	PolicyData.HasElevator = !pre;
}

void APolicy::SetHasBicycle( ) {
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
}

void APolicy::SetHasTransfer( ) {
	bool pre = PolicyData.HasTransfer;

	if (!pre) { // 전의 상태가 비활성화면
		// TODO transfer 함수 추가
	}
	else { // 전의 상태가 활성화
		// TODO transfer 함수 추가
	}

	PolicyData.HasTransfer = !pre;

}


int APolicy::GetServiceCostLevel() {
	return PolicyData.ServiceCostLevel;
}

bool APolicy::GetHandicappedSeat() {
	return PolicyData.HandicappedSeat;
}

bool APolicy::GetHasCCTV() {
	return PolicyData.HasCCTV;
}

bool APolicy::GetHasElevator() {
	return PolicyData.HasElevator;
}

bool APolicy::GetHasBicycle() {
	return PolicyData.HasBicycle;
}

bool APolicy::GetHasTransfer() {
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
	if (GetHasCCTV()) {
		return 5;
	}
	else {
		return 0;
	}
}

int APolicy::GetCostForElevator() {
	if (GetHasElevator()) {
		return 10;
	}
	else {
		return 0;
	}
}


int32 APolicy::CalculateComplainPercentage() {

	int32 currentPercentage = 0; //음수면 감소량

	if (GetHandicappedSeat()) {
		currentPercentage -= 5;
	}

	if (GetHasCCTV()) {
		currentPercentage -= 10;
	}

	if (GetHasElevator()) {
		currentPercentage -= 15;
	}

	if (GetHasBicycle()) {
		currentPercentage += 10;
	}

	if (GetHasTransfer()) {
		currentPercentage -= 20;
	}

	return currentPercentage;
}


void APolicy::InitPolicy() {
}