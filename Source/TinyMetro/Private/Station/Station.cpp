// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Station.h"
#include "Station/StationManager.h"
#include "Station/PathQueue.h"
#include "Station/StationInfoWidget.h"
#include "Station/StationSaveGame.h"
#include "Sound/SoundManager.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Train/TrainTemplate.h"
#include "Timer/Timer.h"
#include "SaveSystem/TMSaveManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Statistics/StatisticsManager.h"
#include "Lane/LaneManager.h"
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

	OnInputTouchBegin.AddDynamic(this, &AStation::StationTouchBegin);
	OnInputTouchEnd.AddDynamic(this, &AStation::StationTouchEnd);
}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{
	Super::BeginPlay();
	
	// Get GameMode, set daytime
	if (!GameModeRef) {
		GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	}
	StationManager = GameModeRef->GetStationManager();
	TimerRef = GameModeRef->GetTimer();
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	PolicyRef = GameModeRef->GetPolicy();
	StatisticsManagerRef = GameModeRef->GetStatisticsManager();
	LaneManagerRef = GameModeRef->GetLaneManager();
	SoundManagerRef = GameModeRef->GetSoundManager();

	StationMeshComponent->SetStaticMesh(StationMesh[(int)StationInfo.Type]);
	StationComplainMeshComponent->SetStaticMesh(StationComplainMesh[(int)StationInfo.Type]);
	InitComplainGauge();
	UpdateStationMesh();
	UpdatePassengerMesh();
	SetInfoWidget(StationManager->GetStationInfoWidget());

	if (StationInfo.IsUpgrade) {
		Upgrade();
	}

	TimerRef->DailyTask.AddDynamic(this, &AStation::DailyTask);
	TimerRef->WeeklyTask.AddDynamic(this, &AStation::WeeklyTask);

	PolicyRef->PolicyUpdateTask.AddDynamic(this, &AStation::UpdatePolicy);

	GameModeRef->GetSaveManager()->SaveTask.AddDynamic(this, &AStation::Save);
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

	// Update passenger wait time
	for (auto& i : Passenger) {
		i.WaitTime += DeltaTime;
	}
}

void AStation::SetStationId(int32 Id) {
	StationInfo.Id = Id;
}

int32 AStation::GetStationId() const {
	return StationInfo.Id;
}

void AStation::SetStationType(StationType Type) {
	StationInfo.Type = Type;
	StationTypeValue = Type;
}

void AStation::SetGridCellData(FGridCellData _GridCellData) {
	CurrentGridCellData = _GridCellData;
}

FGridCellData AStation::GetCurrentGridCellData() const {
	return CurrentGridCellData;
}

void AStation::WeeklyTask() {
	// Init values
	StationInfo.WeeklyProfit = 0;
	StationInfo.WeeklyTransferPassenger = 0;
	StationInfo.WeeklyUsingPassenger = 0;

	// Maintenance cost
	PlayerStateRef->AddIncome(-MaintenanceCost - PolicyServiceLevel.WeeklyCost);
}

void AStation::DailyTask() {
	// Add spawn day
	SpawnDay++;
	// Update complain
	ComplainRoutine();
	// Off spawn pulse;
	OffSpawnAlarm();
}

void AStation::Save() {
	UStationSaveGame* tmp = Cast<UStationSaveGame>(UGameplayStatics::CreateSaveGameObject(UStationSaveGame::StaticClass()));
	tmp->Passenger = Passenger;
	tmp->PassengerSpawnCurrent = PassengerSpawnCurrent;
	tmp->SpawnDay = SpawnDay;
	tmp->StationInfo = StationInfo;

	GameModeRef->GetSaveManager()->Save(tmp, SaveActorType::Station, StationInfo.Id);
}

void AStation::Load() {
	if (!GameModeRef) {
		GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	}
	UStationSaveGame* tmp = Cast<UStationSaveGame>(
		GameModeRef->GetSaveManager()->Load(SaveActorType::Station, StationInfo.Id));

	// Load success
	if (IsValid(tmp)) {
		Passenger = tmp->Passenger;
		PassengerSpawnCurrent = tmp->PassengerSpawnCurrent;
		SpawnDay = tmp->SpawnDay;
		StationInfo = tmp->StationInfo;
	}
}

// Station -> Train
FPassenger AStation::GetOnPassenger(int32 Index, ATrainTemplate* Train) {
	// TPair.key : Passenger pointer
	// TPair.value : Index validation (true : Need to next passenger check, false : Last index)
	//TPair<UPassenger*, bool> RidePassenger(nullptr, false);
	for (int i = 0; i < Passenger.Num(); i++) {
		// Update passenger route
		auto PassengerRoute = StationManager->GetShortestPath(
			StationInfo.Id,
			Passenger[i].Destination
		);

		// Check route is empty
		if (!PassengerRoute.IsEmpty()) {
			// Passenger next stopover station == Train next station
			// True : Passenger get on
			if (PassengerRoute.Peek() == Train->GetNextStation().Id) {
				PassengerRoute.Dequeue();
				FPassenger tmp = Passenger[i];
				Passenger.RemoveAt(i);
				UpdatePassengerMesh();

				// Get money
				if (!tmp.IsAlreadyPaid && !tmp.IsFree) {
					PlayerStateRef->AddMoney(Fare);
					StationInfo.TotalProfit += Fare;
					StationInfo.WeeklyProfit += Fare;

					{
						FScopeLock lock(StatisticsManagerRef->GetLaneStatisticsKey().Pin().Get());
						StatisticsManagerRef->LaneStatistics.Lanes[Train->GetServiceLaneId()].TotalProfit += Fare;
						StatisticsManagerRef->LaneStatistics.Lanes[Train->GetServiceLaneId()].WeeklyProfit += Fare;
					}
				}
				tmp.IsAlreadyPaid = true;

				// Update wait time
				StationInfo.GetOnPassengerCount++;
				StationInfo.TotalWaitTime += tmp.WaitTime;
				StationInfo.AverageWaitTime = StationInfo.TotalWaitTime / StationInfo.GetOnPassengerCount;
				tmp.WaitTime = 0.0f;

				return MoveTemp(tmp);
			}
		}

		//Passenger[i]->SetPassengerPath(PassengerRoute);
	}

	FPassenger tmp;
	tmp.Destination = StationType::None;
	return tmp;
}

// Train -> Station
void AStation::GetOffPassenger(FPassenger P, ATrainTemplate* Train) {
	if (P.Destination == this->StationInfo.Type) {
		// Passenger arrive destination
		{
			FScopeLock lock(StatisticsManagerRef->GetLaneStatisticsKey().Pin().Get());
			StatisticsManagerRef->LaneStatistics.Lanes[Train->GetServiceLaneId()].TotalArrivePassenger++;
			StatisticsManagerRef->LaneStatistics.Lanes[Train->GetServiceLaneId()].WeeklyArrivePassenger++;
		}
		P.Destination = StationType::None;
	} else {
		Passenger.Add(P);

		// Update Passenger using count
		StationInfo.TotalUsingPassenger++;
		StationInfo.WeeklyUsingPassenger++;
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

void AStation::SpawnPassenger(StationType Destination) {
	FPassenger tmp;
	tmp.Destination = Destination;
	tmp.IsFree = StationManager->CalculateFreePassegnerSpawnProbability();

	// Update Passenger using count
	StationInfo.TotalUsingPassenger++;
	StationInfo.WeeklyUsingPassenger++;

	Passenger.Add(MoveTemp(tmp));
	UpdatePassengerMesh();
}

void AStation::DespawnPassenger(StationType Destination) {
	for (int i = 0; i < Passenger.Num(); i++) {
		if (Passenger[i].Destination == Destination) {
			Passenger.RemoveAt(i);
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

int32 AStation::GetWaitPassenger() const {
	return Passenger.Num();
}

void AStation::SetActivate(bool Flag) {
	IsActive = Flag;
	State = Flag ? StationState::Active : StationState::Inactive;
	StationInfo.IsActive = true;

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
	ComplainDynamicMaterial->SetScalarParameterValue("Gauge", StationInfo.Complain / ComplainMax);

	StationComplainMeshComponent->SetMaterial(0, ComplainDynamicMaterial);
}

void AStation::SetComplainGauge(float Per) {
	ComplainDynamicMaterial->SetScalarParameterValue("Gauge", Per);
}

void AStation::SetComplainIncreaseEnable(bool Flag) {
	IsComplainIncreaseEnable = Flag;
}

void AStation::ScaleComplain(float Rate) {
	StationInfo.Complain *= Rate;
}

void AStation::AddComplain(float Value, bool IsFixedValue) {
	StationInfo.Complain += (Value * (IsFixedValue ? 1.0f : StationManager->GetComplainIncreaseRate()));
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
	switch (S) {
	case StationState::Inactive:
		StationInfo.IsActive = false;
		StationInfo.IsDestroyed = false;
		break;
	case StationState::Active:
		StationInfo.IsActive = true;
		StationInfo.IsDestroyed = false;
		break;
	case StationState::Destroyed:
		StationInfo.IsActive = false;
		StationInfo.IsDestroyed = true;
		break;
	default:
		break;
	}
	UpdateStationMesh();
}

StationType AStation::GetStationType() const {
	return StationInfo.Type;
}

float AStation::GetComplain() const {
	return StationInfo.Complain;
}

TArray<int32> AStation::GetLanes()
{
	return Lanes;
}

void AStation::SetLanes(int32 AdditionalLaneId)
{
	Lanes.Add(AdditionalLaneId);
	StationInfo.ServiceLaneCount = Lanes.Num();
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
	if (!StationInfo.IsUpgrade) {
		SoundManagerRef->PlaySound(TinyMetroEffectSound::StationUpgrade);
	}
	StationInfo.IsUpgrade = true;
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

bool AStation::CanUpgrade() const {
	if (!StationInfo.IsUpgrade && PlayerStateRef->GetMoney() >= UpgradeCost) return true;
	else return false;
}

void AStation::SetInfoWidget(UStationInfoWidget* Widget) {
	StationInfoWidget = Widget;
}

void AStation::StationOnPressed(AActor* Target, FKey ButtonPressed) {
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Station::ClickBegin C++"));
	TouchTime = 0.0f;
}

void AStation::StationOnReleased(AActor* Target, FKey ButtonPressed) {
	if (TouchTime > LongClickInterval) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Station::ClickEnd::Long C++"));
		StationInfoWidget->ShowWidget(this);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Station::ClickEnd::Short C++"));
		LaneManagerRef->AddSelectedStations(this);
	}
}

void AStation::StationTouchBegin(ETouchIndex::Type FingerIndex, AActor* TouchedActor) {
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Station::TouchBegin C++"));
	TouchTime = 0.0f;
}

void AStation::StationTouchEnd(ETouchIndex::Type FingerIndex, AActor* TouchedActor) {
	if (TouchTime > LongClickInterval) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Station::TouchEnd::Long C++"));
		StationInfoWidget->ShowWidget(this);
	} else {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Station::TouchEnd::Short C++"));
		LaneManagerRef->AddSelectedStations(this);
	}
}

void AStation::ComplainRoutine() {
	// Check station can increase complain
	if (IsComplainIncreaseEnable) {
		// Complain from Passenger
		if (Passenger.Num() > ComplainPassengerNum) {
			StationInfo.Complain += (ComplainFromPassenger * (Passenger.Num() - ComplainPassengerNum)) * StationManager->GetComplainIncreaseRate();
		}

		// Complain from not activate
		if (!IsActive && SpawnDay > ComplainSpawnDay) {
			StationInfo.Complain += ComplainFromInactive * StationManager->GetComplainIncreaseRate();
		}

		// Compalin from service level
		StationInfo.Complain += PolicyServiceLevel.DailyComplain;
	}
	// Update Complain gauge Mesh
	SetComplainGauge(StationInfo.Complain / ComplainMax);


	// Complain excess : Game over
	if (ComplainMax <= StationInfo.Complain) {
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
			PassengerMeshComponent[i]->SetStaticMesh(PassengerMesh[(int)Passenger[i].Destination]);
		} else {
			PassengerMeshComponent[i]->SetStaticMesh(nullptr);
		}
	}
}

void AStation::PassengerSpawnRoutine(float DeltaTime) {
	PassengerSpawnCurrent += DeltaTime * PassengerSpawnSpeed;
	// Check tick (Time)
	if (PassengerSpawnCurrent >= PassengerSpawnRequire) {
		// Check spawn enable
		if (IsPassengerSpawnEnable && !StationInfo.IsDestroyed) {
			// Check spawn probability
			if (StationManager->CalculatePassegnerSpawnProbability()) {
				SpawnPassenger(StationManager->CalculatePassengerDestination(StationInfo.Type));
			}
		}
		PassengerSpawnCurrent -= PassengerSpawnRequire;
	}
}

void AStation::OffSpawnAlarm() {
	if (SpawnAlarm) {
		SpawnAlarm = false;
		PulseComponent->SetMaterial(0, nullptr);
		PulseComponent->SetWorldScale3D(FVector(0));
	}
}

void AStation::EventEnd() {
}

void AStation::UpdatePolicy() {
	auto policyData = PolicyRef->GetPolicyData();
	PolicyServiceLevel = PolicyRef->ServiceLevel[policyData.ServiceCostLevel];
	MaintenanceCost = 0;

	MaintenanceCost += PolicyServiceLevel.WeeklyCost;

	if (policyData.HasCCTV) {
		MaintenanceCost += 5;
	}

	if (policyData.HasElevator) {
		MaintenanceCost += 10;
	}

	if (policyData.HasTransfer) {
		TransferStation = true;
	}
}

// Not used
// Replcaed by SpawnPassenger(StationType)
void AStation::SpawnPassenger() {
	/*auto NewPassengerDestination = StationManager->CalculatePassengerDest(StationInfo.Type);
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

	
	Passenger.Add(MoveTemp(tmp));*/

	//Log
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Passenger Spawn!")));*/
}


