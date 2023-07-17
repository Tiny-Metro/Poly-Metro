// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Station.h"
#include "Station/StationManager.h"
#include "Station/PathQueue.h"
#include "Station/StationInfoWidget.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Train/TrainTemplate.h"
#include "Timer/Timer.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Components/BoxComponent.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>

// Sets default values
AStation::AStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Set root
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	// Set station mesh
	StationMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh"));
	StationMeshComponent->SetupAttachment(RootComponent);
	StationMeshComponent->SetGenerateOverlapEvents(false);

	// Set station complain mesh
	StationComplainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Complain Mesh"));
	StationComplainMeshComponent->SetupAttachment(RootComponent);
	StationComplainMeshComponent->SetGenerateOverlapEvents(false);
	StationComplainMeshComponent->SetWorldScale3D(FVector(1.20f));
	
	// Set station pulse effect mesh
	PulseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pulse plane"));
	PulseComponent->SetupAttachment(RootComponent);
	PulseComponent->SetGenerateOverlapEvents(false);
	PulseComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object);
	PulseComponent->SetWorldScale3D(FVector(20.0f, 20.0f, 1.0f));

	// Set passenger mesh
	for (int i = 0; i < MaxPassengerSpawn; i++) {
		FName name = *FString::Printf(TEXT("Passenger %d"), i);
		auto tmp = CreateDefaultSubobject<UStaticMeshComponent>(name);
		tmp->SetGenerateOverlapEvents(false);
		tmp->SetupAttachment(RootComponent);
		FVector PassengerPosition =
			PassengerMeshDefaultPosition +
			FVector(PassengerX_Distance * (i / 2), 0.0f, 0.0f) +
			FVector(0.0f, PassengerY_Distance * ((i % 2) == 0 ? -1 : 1), 0.0f);
		if ((i == MaxPassengerSpawn - 1) && (MaxPassengerSpawn % 2 == 1)) {
			PassengerPosition += FVector(0.0f, PassengerY_Distance * ((i % 2) == 0 ? 1 : -1), 0.0f);
		}
		tmp->SetRelativeLocation(PassengerPosition);
		PassengerMeshComponent.Add(MoveTemp(tmp));
	}
	
	// Load meshes (Station)
	for (auto& i : StationMeshPath) {
		StationMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}

	// Load meshes (Passenger)
	for (auto& i : PassengerMeshPath) {
		PassengerMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}
	
	// Load meshes (Complain)
	for (auto& i : ComplainMeshPath) {
		StationComplainMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}

	// Load material (Station)
	for (auto& i : StationMaterialInactivePath) {
		StationMaterialInactive.AddUnique(ConstructorHelpers::FObjectFinder<UMaterial>(*i).Object);
	}
	for (auto& i : StationMaterialActivePath) {
		StationMaterialActive.AddUnique(ConstructorHelpers::FObjectFinder<UMaterial>(*i).Object);
	}
	for (auto& i : StationMaterialDestroyedPath) {
		StationMaterialDestroyed.AddUnique(ConstructorHelpers::FObjectFinder<UMaterial>(*i).Object);
	}

	// Load material (Complain)
	ComplainMaterial = ConstructorHelpers::FObjectFinder<UMaterialInterface>(*ComplainMaterialPath).Object;

	// Set overlap volume
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	OverlapVolume->InitBoxExtent(FVector(10, 10, 100));
	OverlapVolume->SetupAttachment(RootComponent);

	// Bind click, release event
	OnClicked.AddDynamic(this, &AStation::StationOnPressed);
	OnReleased.AddDynamic(this, &AStation::StationOnReleased);
}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{

	Super::BeginPlay();
	
	// Get GameMode, set daytime
	ATinyMetroGameModeBase* GameMode = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	StationManager = GameMode->GetStationManager();
	Daytime = GameMode->GetDaytime();
	TimerRef = GameMode->GetTimer();
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	StationMeshComponent->SetStaticMesh(StationMesh[(int)StationInfo.Type]);
	StationComplainMeshComponent->SetStaticMesh(StationComplainMesh[(int)StationInfo.Type]);
	InitComplainGauge();
	UpdateStationMesh();
	UpdatePassengerMesh();

	// Set off alarm pulse
	if (SpawnAlarm) {
		FTimerHandle alarmHandle;
		GetWorld()->GetTimerManager().SetTimer(alarmHandle, this, &AStation::OffSpawnAlarm, Daytime);
	}

	TimerRef->DailyTask.AddDynamic(this, &AStation::DailyTask);
	TimerRef->WeeklyTask.AddDynamic(this, &AStation::WeeklyTask);
}

// Called every frame
void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PassengerSpawnRoutine(DeltaTime);

	// Add TouchTime
	// If condition don't needed
	// (When OnClick, TouchTime init by 0.0)
	TouchTime += DeltaTime;
}

void AStation::SetStationId(int32 Id) {
	StationId = Id;
}

int32 AStation::GetStationId() const {
	return StationInfo.Id;
}

void AStation::SetStationType(StationType Type) {
	StationInfo.Type = Type;
}

void AStation::SetGridCellData(FGridCellData _GridCellData) {
	CurrentGridCellData = _GridCellData;
}

void AStation::SetPolicy(APolicy* _Policy) {
	Policy = _Policy;
}

FGridCellData AStation::GetCurrentGridCellData() const {
	return CurrentGridCellData;
}

void AStation::WeeklyTask() {
}

void AStation::DailyTask() {
	// Add spawn day
	SpawnDay++;
	// Update complain
	ComplainRoutine();
}

UPassenger* AStation::GetOnPassenger(int32 Index, ATrainTemplate* Train) {
	// TPair.key : Passenger pointer
	// TPair.value : Index validation (true : Need to next passenger check, false : Last index)
	//TPair<UPassenger*, bool> RidePassenger(nullptr, false);
	for (int i = 0; i < Passenger.Num(); i++) {
		// Update passenger route
		auto PassengerRoute = StationManager->GetShortestPath(
			StationInfo.Id,
			Passenger[i]->GetDestination()
		);

		// Check route is empty
		if (!PassengerRoute.IsEmpty()) {
			// Passenger next stopover station == Train next station
			// True : Passenger get on
			if (PassengerRoute.Peek() == Train->GetNextStation().Id) {
				PassengerRoute.Dequeue();
				UPassenger* tmp = Passenger[i];
				Passenger.RemoveAt(i);
				UpdatePassengerMesh();
				
				return MoveTemp(tmp);
			}
		}

		Passenger[i]->SetPassengerPath(PassengerRoute);
	}

	return nullptr;
}

void AStation::GetOffPassenger(UPassenger* P) {
	if (P->GetDestination() == this->StationInfo.Type) {
		// Passenger arrive destination
		if (P->GetFree()) {
			ArriveFreePassenger++;
		} else {
			ArrivePaidPassenger++;
		}
		P = nullptr;
	} else {
		Passenger.Add(P);
	}
	UpdatePassengerMesh();
}
  
bool AStation::IsValidLane(int32 LId) const
{
	return false;
}

void AStation::RemoveLane(int32 LId)
{
	Lanes.Remove(LId);
}

TMap<StationType, int32> AStation::GetSpawnPassengerStatistics() const {
	return TotalSpawnPassenger;
}

TMap<StationType, int32> AStation::GetSpawnTotalPassenger() const {
	TMap<StationType, int32> result;
	for (auto i : SpawnPaidPassenger) {
		result[i.Key] += i.Value;
	}
	for (auto i : SpawnFreePassenger) {
		result[i.Key] += i.Value;
	}
	return result;
}

TMap<StationType, int32> AStation::GetSpawnPaidPassenger() const {
	return SpawnPaidPassenger;
}

TMap<StationType, int32> AStation::GetSpawnFreePassenger() const {
	return SpawnFreePassenger;
}

int32 AStation::GetArriveTotalPassengerCount() const {
	return GetArrivePaidPassengerCount() + GetArriveFreePassengerCount();
}

int32 AStation::GetArrivePaidPassengerCount() const {
	return ArrivePaidPassenger;
}

int32 AStation::GetArriveFreePassengerCount() const {
	return ArriveFreePassenger;
}

int32 AStation::GetWaitTotalPassengerCount() const {
	return GetWaitFreePassengerCount() + GetWaitPaidPassengerCount();
}

int32 AStation::GetWaitPaidPassengerCount() const {
	int32 result = 0;
	for (auto& i : Passenger) {
		if (!i->GetFree()) {
			result++;
		}
	}
	return result;
}

int32 AStation::GetWaitFreePassengerCount() const {
	int32 result = 0;
	for (auto& i : Passenger) {
		if (i->GetFree()) {
			result++;
		}
	}
	return result;
}

TMap<StationType, int32> AStation::GetDestroyedTotalPassenger() const {
	TMap<StationType, int32> result;
	for (auto i : DestroyedPaidPassenger) {
		result[i.Key] += i.Value;
	}
	for (auto i : DestroyedFreePassenger) {
		result[i.Key] += i.Value;
	}
	return result;
}

TMap<StationType, int32> AStation::GetDestroyedPaidPassenger() const {
	return DestroyedPaidPassenger;
}

TMap<StationType, int32> AStation::GetDestroyedFreePassenger() const {
	return DestroyedFreePassenger;
}

int32 AStation::GetWaitPassenger() const {
	return Passenger.Num();
}

void AStation::SpawnPassenger(StationType Destination) {
	UPassenger* tmp = UPassenger::ConstructPassenger(
		Destination
	);
	//tmp->SetPassengerRoute(StationManager->GetShortestRoute(StationInfo.Id, NewPassengerDestination));
	//UPassenger* tmp = NewObject<UPassenger>();
	//tmp->SetDestination(StationManager->CalculatePassengerDest(StationInfo.Type));
	if (FMath::RandRange(0.0, 1.0) < FreePassengerSpawnProbability) {
		tmp->SetFree();
	}

	if (tmp->GetFree()) {
		SpawnFreePassenger[tmp->GetDestination()]++;
		StationManager->NotifySpawnPassenger(tmp->GetDestination(), true);
	} else {
		SpawnPaidPassenger[tmp->GetDestination()]++;
		StationManager->NotifySpawnPassenger(tmp->GetDestination(), false);
	}
	TotalSpawnPassenger[tmp->GetDestination()]++;


	Passenger.Add(MoveTemp(tmp));
	UpdatePassengerMesh();
}

void AStation::DespawnPassenger(StationType Destination) {
	for (auto& i : Passenger) {
		if (i->GetDestination() == Destination) {
			Passenger.Remove(i);
			break;
		}
	}
	UpdatePassengerMesh();
}

void AStation::DespawnRandomPassenger() {
	int64 removeIndex = FMath::RandRange(0, Passenger.Num() - 1);
	if (Passenger.IsValidIndex(removeIndex)) {
		Passenger.RemoveAt(removeIndex);
	}
	UpdatePassengerMesh();
}

void AStation::SetPassengerSpawnEnable(bool Flag) {
	IsPassengerSpawnEnable = Flag;
}

bool AStation::GetPassengerSpawnEnable() const {
	return IsPassengerSpawnEnable;
}

void AStation::CalculateComplain() {
}

void AStation::SetActivate(bool Flag) {
	IsActive = Flag;
	State = Flag ? StationState::Active : StationState::Inactive;
	
	UpdateStationMesh();

	// TODO :  Visible logic
}

void AStation::UpdateComplainMesh() {
}

void AStation::InitComplainGauge() {
	// Create a dynamic material instance
	ComplainDynamicMaterial = UMaterialInstanceDynamic::Create(ComplainMaterial, this);

	// Set the GaugePer value
	//GaugePer = 0.0f; // Replace this with the value you want to set
	ComplainDynamicMaterial->SetScalarParameterValue("Gauge", ComplainCurrent / ComplainMax);

	StationComplainMeshComponent->SetMaterial(0, ComplainDynamicMaterial);
}

void AStation::SetComplainGauge(float Per) {
	ComplainDynamicMaterial->SetScalarParameterValue("Gauge", Per);
}

void AStation::AddComplainIncreaseRate(float Rate, int32 Period) {
	ComplainIncreaseRate += Rate;
	/*if (Period != -1) {
		GetWorld()->GetTimerManager().SetTimer(
			TimerComplain,
			FTimerDelegate::CreateLambda([&]() {
				AdditionalPassengerSpawnProbability -= Rate;
				}),
			Period,
			false,
			0.0f
		);
	}*/
}

void AStation::SetComplainIncreaseEnable(bool Flag) {
	IsComplainIncreaseEnable = Flag;
}

void AStation::SetComplainByRate(float Rate) {
	ComplainCurrent *= Rate;
}

void AStation::AddComplain(float Value, bool IsFixedValue) {
	ComplainCurrent += (Value * (IsFixedValue ? 1.0f : ComplainIncreaseRate));
}

void AStation::MaintenanceCost(int32 Cost) {

}

void AStation::UpdateStationMesh() {
	switch (State) {
	case StationState::Inactive :
		StationMeshComponent->SetMaterial(0, StationMaterialInactive[0]);
		StationMeshComponent->SetMaterial(1, StationMaterialInactive[1]);
		break;
	case StationState::Active :
		StationMeshComponent->SetMaterial(0, StationMaterialActive[0]);
		StationMeshComponent->SetMaterial(1, StationMaterialActive[1]);
		break;
	case StationState::Destroyed : 
		StationMeshComponent->SetMaterial(0, StationMaterialDestroyed[0]);
		StationMeshComponent->SetMaterial(1, StationMaterialDestroyed[1]);
		break;
	}
}

StationState AStation::GetStationState() const {
	return State;
}

void AStation::SetStationState(StationState S) {
	State = S;
	UpdateStationMesh();
}

StationType AStation::GetStationType() const {
	return StationInfo.Type;
}

void AStation::AddComplain(double ReduceRate) {
	ComplainCurrent *= ReduceRate;
}

void AStation::AddComplain(int32 ReduceValue) {
	ComplainCurrent += (ReduceValue * ComplainIncreaseRate);
}

int32 AStation::GetComplain() const {
	return ComplainCurrent;
}

TArray<int32> AStation::GetLanes()
{
	return Lanes;
}

void AStation::SetLanes(int32 AdditionalLaneId)
{
	Lanes.Add(AdditionalLaneId);
}

void AStation::SetTransfer(bool Flag) {
	TransferStation = Flag;
}

FStationInfo AStation::GetStationInfo()
{
	return StationInfo;
}

void AStation::SetStationInfo(int32 Id, StationType Type)
{
	StationInfo.Id = Id;
	StationInfo.Type = Type;
}

void AStation::Upgrade() {
	if (CanUpgrade()) {
		PlayerStateRef->AddMoney(-UpgradeCost);
		IsUpgrade = true;
		ComplainPassengerNum += UpgradePermissionComplainPassenger;

		auto meshScale = StationMeshComponent->GetComponentScale();
		meshScale.X *= 1.5f;
		meshScale.Y *= 1.5f;
		StationMeshComponent->SetWorldScale3D(meshScale);

		auto gaugeScale = StationComplainMeshComponent->GetComponentScale();
		gaugeScale.X *= 1.5f;
		gaugeScale.Y *= 1.5f;
		StationComplainMeshComponent->SetWorldScale3D(gaugeScale);

		for (auto& i : PassengerMeshComponent) {
			i->AddRelativeLocation(FVector(70.0f, 0, 0));
		}
	}
}

bool AStation::CanUpgrade() const {
	if (!IsUpgrade && PlayerStateRef->GetMoney() >= UpgradeCost) return true;
	else return false;
}

void AStation::SetInfoWidget(UStationInfoWidget* Widget) {
	StationInfoWidget = Widget;
}

void AStation::StationOnPressed(AActor* Target, FKey ButtonPressed) {
	TouchTime = 0.0f;
}

void AStation::StationOnReleased(AActor* Target, FKey ButtonPressed) {
	if (TouchTime > LongClickInterval) {
		StationInfoWidget->ShowWidget(this);
	}
}

void AStation::ComplainRoutine() {
	// Check station can increase complain
	if (IsComplainIncreaseEnable) {
		// Complain from Passenger
		if (Passenger.Num() > ComplainPassengerNum) {
			ComplainCurrent += (ComplainFromPassenger * (Passenger.Num() - ComplainPassengerNum)) * ComplainIncreaseRate;
		}

		// Complain from not activate
		if (!IsActive && SpawnDay > ComplainSpawnDay) {
			ComplainCurrent += ComplainFromInactive * ComplainIncreaseRate;
		}
	}
	// Update Complain gauge Mesh
	SetComplainGauge(ComplainCurrent / ComplainMax);


	// Complain excess : Game over
	if (ComplainMax <= ComplainCurrent) {
		// Game over code

		//Log
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.0f,
				FColor::Red,
				FString::Printf(TEXT("Game Over")));
	}
}

void AStation::UpdatePassengerMesh() {
	// Read passenger array, clear and reorganize meshes
	for (int i = 0; i < MaxPassengerSpawn; i++) {
		if (Passenger.IsValidIndex(i)) {
			PassengerMeshComponent[i]->SetStaticMesh(PassengerMesh[(int)Passenger[i]->GetDestination()]);
		} else {
			PassengerMeshComponent[i]->SetStaticMesh(nullptr);
		}
	}
}

void AStation::PassengerSpawnRoutine(float DeltaTime) {
	PassengerSpawnCurrent += DeltaTime * PassengerSpawnSpeed;
	if (PassengerSpawnCurrent >= PassengerSpawnRequire) {
		if (IsPassengerSpawnEnable) {
			if (FMath::RandRange(0.0, 1.0) > GetPassengerSpawnProbability()) {
				SpawnPassenger(StationManager->CalculatePassengerDest(StationInfo.Type));
			}
		}
		PassengerSpawnCurrent -= PassengerSpawnRequire;
	}
}

void AStation::OffSpawnAlarm() {
	SpawnAlarm = false;
	PulseComponent->SetMaterial(0, nullptr);
	PulseComponent->SetWorldScale3D(FVector(0));
}

// Not used
// Replcaed by SpawnPassenger(StationType)
void AStation::SpawnPassenger() {
	auto NewPassengerDestination = StationManager->CalculatePassengerDest(StationInfo.Type);
	UPassenger* tmp = UPassenger::ConstructPassenger(
		NewPassengerDestination
	);

	if (FMath::RandRange(0.0, 1.0) < FreePassengerSpawnProbability) {
		tmp->SetFree();
	}
	
	if (tmp->GetFree()) {
		SpawnFreePassenger[tmp->GetDestination()]++;
		StationManager->NotifySpawnPassenger(tmp->GetDestination(), true);
	} else {
		SpawnPaidPassenger[tmp->GetDestination()]++;
		StationManager->NotifySpawnPassenger(tmp->GetDestination(), false);
	}
	TotalSpawnPassenger[tmp->GetDestination()]++;

	
	Passenger.Add(MoveTemp(tmp));

	//Log
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Passenger Spawn!")));*/
}

double AStation::GetPassengerSpawnProbability() {
	
	return PassengerSpawnProbability * AdditionalPassengerSpawnProbability;
}


