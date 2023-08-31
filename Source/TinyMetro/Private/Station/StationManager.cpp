// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationManager.h"
#include "Lane/Lane.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationInfo.h"
#include "Timer/Timer.h"
#include "Station/StationInfoWidget.h"
#include "Station/StationSpawnBorderWidget.h"
#include "Station/StationManagerSaveGame.h"
#include "Statistics/StatisticsManager.h"
#include "Policy/Policy.h"
#include "Event/TinyMetroEventManager.h"
#include "SaveSystem/TMSaveManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Blueprint/UserWidget.h>
#include <Engine/AssetManager.h>

// Sets default values
AStationManager::AStationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init GridManager
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	AdjList = NewObject<UAdjList>();

	// Init Floyd-Warshall
	// Init adj matrix
	FAdjArray adjTmp;
	adjTmp.Init(TNumericLimits<float>::Max(), 301);
	adj.Init(adjTmp, 301);

	// Init path array
	FPath pathTmp;
	pathTmp.Init(-1, 301);
	AdjPath.Init(pathTmp, 301);
	AdjDist.Init(adjTmp, 301);
}

// Called when the game starts or when spawned
void AStationManager::BeginPlay()
{
	Super::BeginPlay();

	// Init default data
	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	SaveManagerRef = GameMode->GetSaveManager();
	TimerRef = GameMode->GetTimer();
	PolicyRef = GameMode->GetPolicy();
	StatisticsManagerRef = GameMode->GetStatisticsManager();

	auto GridSize = GridManager->GetGridSize();
	int32 SpawnPrevent = GridManager->GetStationSpawnPrevent();

	if (IsValid(PlayerState)) {
		UE_LOG(LogTemp, Log, TEXT("PlayerState Valid"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("PlayerState Invalid"));
	}

	// Init info widget
	InitStationInfoWidget();

	// Init spawn alarm widget
	InitStationSpawnWidget();

	// Load initializa station data
	// Need to modify by SaveManager

	/*if (!Load()) {
		UE_LOG(LogTemp, Log, TEXT("StationManager::BeginPlay::No save data"));
		InitData = GameMode->GetInitData();

		for (auto& i : InitData) {
			SpawnStation(GridManager->GetGridCellDataByPoint(i.Key.X, i.Key.Y), i.Value);
		}
	} else {
		UE_LOG(LogTemp, Log, TEXT("StationManager::BeginPlay::Load data"));
	}*/

	// New game
	if (Station.Num() == 0) {
		InitData = GameMode->GetInitData();

		for (auto& i : InitData) {
			SpawnStation(GridManager->GetGridCellDataByPoint(i.Key.X, i.Key.Y), i.Value);
		}
	}

	// Register Timer tasks
	GameMode->GetTimer()->DailyTask.AddDynamic(this, &AStationManager::DailyTask);
	GameMode->GetTimer()->WeeklyTask.AddDynamic(this, &AStationManager::WeeklyTask);

	// Register Policy update
	PolicyRef->PolicyUpdateTask.AddDynamic(this, &AStationManager::UpdatePolicy);

	// Register Event end
	GameMode->GetEventManager()->EventEndTask.AddDynamic(this, &AStationManager::EventEnd);

	SaveManagerRef->SaveTask.AddDynamic(this, &AStationManager::Save);
}

void AStationManager::TestFunction() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT(":)")));
}

void AStationManager::AddPassegnerSpawnProbabilityByEvent(float Amount) {
	AdditionalPassengerSpawnProbabilityByEvent += Amount;
}

bool AStationManager::CalculatePassegnerSpawnProbability() const {
	if (FMath::RandRange(0.0f, 1.0f) >= (PassengerSpawnProbability + AdditionalPassengerSpawnProbabilityByEvent + AdditionalPassengerSpawnProbabilityByPolicy)) {
		return true;
	} else {
		return false;
	}
}

bool AStationManager::CalculateFreePassegnerSpawnProbability() const {
	if (FMath::RandRange(0.0f, 1.0f) >= (FreePassengerSpawnProbability + FreePassengerSpawnProbabilityByEvent + FreePassengerSpawnProbabilityByPolicy)) {
		return true;
	} else {
		return false;
	}
}

void AStationManager::AddPassengerDestinationTypeWeight(StationType Type, float Amount) {
	if (Type == StationType::None) return;
	PassengerDestinationTypeWeight[Type] += Amount;
}

StationType AStationManager::CalculatePassengerDestination(StationType Except) const {
	TArray<StationType> arr;
	
	for (auto& i : Station) {
		auto tmpType = i->GetStationType();
		if (Except != tmpType) {
			for (int count = 0;  count < PassengerDestinationTypeWeight[tmpType] * 100; count++) {
				arr.Add(tmpType);
			}
		}
	}

	return arr[FMath::RandRange(0, arr.Num() - 1)];
}

float AStationManager::GetComplainAverage() {
	int32 ComplainSum = 0;
	for (auto& i : Station) {
		ComplainSum += i->GetComplain();
	}
	return ((float)ComplainSum)/Station.Num();
}

void AStationManager::InitStationInfoWidget() {
	FSoftClassPath MyWidgetClassRef(TEXT("Blueprint'/Game/Stage/UI/HUD/WBP_StationInfoWidget.WBP_StationInfoWidget_C'"));
	if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>()) {
		StationInfoWidget = CreateWidget<UStationInfoWidget>(GetWorld(), MyWidgetClass);
		StationInfoWidget->AddToViewport();
		StationInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

UStationInfoWidget* AStationManager::GetStationInfoWidget() const {
	return StationInfoWidget;
}

void AStationManager::InitStationSpawnWidget() {
	FSoftClassPath MyWidgetClassRef(TEXT("WidgetBlueprint'/Game/Stage/UI/HUD/WBP_SpawnBorderAlarm.WBP_SpawnBorderAlarm_C'"));
	if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>()) {
		StationSpawnWidget = CreateWidget<UStationSpawnBorderWidget>(GetWorld(), MyWidgetClass);
		StationSpawnWidget->AddToViewport();
		//StationSpawnWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

AStation* AStationManager::GetNearestStation(FVector CurrentLocation, class ALane* LaneRef) {
	double Distance = FVector::Dist(CurrentLocation, Station[0]->GetCurrentGridCellData().WorldLocation);
	int StationIndex = 0;
	bool LaneValid = IsValid(LaneRef);

	for (int i = 1; i < Station.Num(); i++) {
		if (LaneValid && !Station[i]->GetLanes().FindByKey<int32>(LaneRef->GetLaneId())) continue;
		double tmp = FVector::Dist(CurrentLocation, Station[i]->GetCurrentGridCellData().WorldLocation);
		if (Distance > tmp) {
			Distance = tmp;
			StationIndex = i;
		}
	}

	return Station[StationIndex];
}

void AStationManager::SpawnStation(FGridCellData GridCellData, StationType Type, int32 Id, FTimestamp Timestamp) {
	// Load BP Class
	//UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Station/BP_Station.BP_Station'")));
	if (!StationBlueprintClass) {
		StationBlueprintClass = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Station/BP_Station.BP_Station'")));
	}

	// Cast to BP
	//UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!GeneratedStationBlueprint) {
		GeneratedStationBlueprint = Cast<UBlueprint>(StationBlueprintClass);
	}
	// Check object validation
	/*if (!SpawnActor) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}*/

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GridCellData.WorldLocation);
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AStation* tmp = Cast<AStation>(GetWorld()->SpawnActorDeferred<AActor>(GeneratedStationBlueprint->GeneratedClass, SpawnTransform));
	int32 StatoinId;

	if (Id != -1) {
		// Load station
		StatoinId = Id;
		tmp->OffSpawnAlarm();
	} else { 
		// New station
		StatoinId = NextStationId++;
	}
	tmp->SetStationInfo(StatoinId, Type);
	tmp->SetGridCellData(GridCellData);
	tmp->SetPassengerSpawnEnable(IsPassengerSpawnEnable);
	tmp->Load();

	tmp->FinishSpawning(SpawnTransform);

	Station.Add(tmp);
	GridManager->SetGridStructure(
		GridCellData.WorldCoordination.X,
		GridCellData.WorldCoordination.Y, 
		GridStructure::Station);
	GridManager->SetGridStation(
		GridCellData.WorldCoordination.X,
		GridCellData.WorldCoordination.Y,
		GridStationStructure::Station);

	FStationSpawnData spawnLog;
	spawnLog.GridCellData = GridCellData;
	spawnLog.Type = Type;
	spawnLog.StationId = tmp->GetStationId();
	if (Id != -1) {
		// Load station
		spawnLog.Timestamp = Timestamp;
	} else {
		// New station
		spawnLog.Timestamp = TimerRef->GetTimestamp();
	}
	StationSpawnLog.Add(spawnLog);
	
	AdjList->Add(tmp->GetStationInfo(), NewObject<UAdjArrayItem>());

	StationSpawnTask.Broadcast();

	/*UE_LOG(LogTemp, Warning, TEXT("StationSpawn GridCellData intpoint: %d / %d"), GridCellData.WorldCoordination.X, GridCellData.WorldCoordination.Y);
	UE_LOG(LogTemp, Warning, TEXT("StationSpawn"));*/

}

void AStationManager::DestroyRandomStation() {
	int index = 0;

	// Select random station not destroyed
	do {
		index = FMath::RandRange(0, Station.Num() - 1);
	} while (!Station[index]->GetStationInfo().IsDestroyed);

	// Destroy
	Station[index]->SetStationState(StationState::Destroyed);
}

void AStationManager::StationSpawnRoutine(float DeltaTime) {
	// Spawn routine
	// Add time
	StationSpawnCurrent += DeltaTime;
	// If time over the require
	if (StationSpawnCurrent >= StationSpawnRequire) {
		// Spawn random station
		SpawnStation(GridManager->GetGridCellDataRandom(), GetRandomStationType());
		
		// Initialize time
		StationSpawnCurrent -= StationSpawnRequire;
	}
}

void AStationManager::AddAdjListItem(AStation* Start, AStation* End, float Length)
{
	int32 StartId = Start->GetStationInfo().Id;
	int32 EndId = End->GetStationInfo().Id;
	(*AdjList)[Start->GetStationInfo()].Add(End->GetStationInfo(), Length);
	(*AdjList)[End->GetStationInfo()].Add(Start->GetStationInfo(), Length);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, FString::Printf(TEXT("%d to %d : %f"), StartId, EndId, Length));

	adj[StartId][EndId] = Length;
	adj[EndId][StartId] = Length;

	FloydWarshall();

	UE_LOG(LogTemp, Warning, TEXT("AddList: StartId : %d / EndId : %d / Length : %f"), Start->GetStationId(), End->GetStationId(), (*AdjList)[End->GetStationInfo()][Start->GetStationInfo()]);
	UE_LOG(LogTemp, Warning, TEXT("AddList: StartId : %d / EndId : %d / Length : %f"), End->GetStationId(), Start->GetStationId(), (*AdjList)[Start->GetStationInfo()][End->GetStationInfo()]);
}

void AStationManager::RemoveAdjListItem(AStation* Start, AStation* End)
{
	(*AdjList)[Start->GetStationInfo()].RemoveRef(End->GetStationInfo());
	(*AdjList)[End->GetStationInfo()].RemoveRef(Start->GetStationInfo());

	FloydWarshall();

	adj[Start->GetStationInfo().Id][End->GetStationInfo().Id] = TNumericLimits<float>::Max();
	adj[End->GetStationInfo().Id][Start->GetStationInfo().Id] = TNumericLimits<float>::Max();


	UE_LOG(LogTemp, Warning, TEXT(" Remove AddList: StartId : %d / EndId : %d / Length : %d"), Start->GetStationId(), End->GetStationId(), (*AdjList)[Start->GetStationInfo()].Num());

}

AStation* AStationManager::GetStationByStationInfo(FStationInfo Info) {
	for (auto& i : Station) {
		if (i->GetStationInfo() == Info) {
			return i;
		}
	}
	return nullptr;
}

AStation* AStationManager::GetStationById(int32 Id) {
	for (auto& i : Station) {
		if (i->GetStationId() == Id) {
			return i;
		}
	}
	return nullptr;
}

TArray<AStation*> AStationManager::GetAllStations() {
	return Station;
}

TArray<FStationInfo> AStationManager::GetAllStationInfo() {
	TArray<FStationInfo> arr;
	for (auto& i : Station) {
		arr.Add(i->GetStationInfo());
	}

	return arr;
}

PathQueue AStationManager::GetShortestPath(int32 Start, StationType Type) {
	if (ShortestPath.Find(Start) == nullptr) {
		return PathQueue();
	}
	if (ShortestPath[Start].Find(Type) == nullptr) {
		return PathQueue();
	}
	return ShortestPath[Start][Type];
}

float AStationManager::GetDefaultPassengerSpawnSpeed() const {
	return DefaultPassengerSpawnSpeed;
}

float AStationManager::GetDefaultPassengerSpawnProbability() const {
	return DefaultPassengerSpawnProbability;
}

float AStationManager::GetDefaultFreePassengerSpawnProbability() const {
	return DefaultFreePassengerSpawnProbability;
}

float AStationManager::GetPassengerSpawnSpeed(StationType Type) const {
	if (Type == StationType::None) {
		return GetDefaultPassengerSpawnSpeed();
	} else {
		float addFactor = 0.0f;
		float multiFactor = 1.0f;

		return multiFactor + addFactor;
	}
}

void AStationManager::AddComplainIncreaseRateByEvent(float Rate) {
	ComplainIncreaseRateByEvent += Rate;
}

float AStationManager::GetComplainIncreaseRate() const {
	return ComplainIncreaseRate + 
		ComplainIncreaseRateByEvent + 
		ComplainIncreaseRateByPolicy;
}

void AStationManager::SetPassengerSpawnEnable(bool Flag) {
	IsPassengerSpawnEnable = Flag;
	for (auto& i : Station) {
		i->SetPassengerSpawnEnable(Flag);
	}
}

bool AStationManager::GetPassengerSpawnEnable() const {
	return IsPassengerSpawnEnable;
}

void AStationManager::WeeklyTask() {
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Active) {
				// Add weekly tasks
				
			}
		}
	}
}

void AStationManager::DailyTask() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("StationManager::DailyTask")));
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Active) {

			}
		}
	}
}

void AStationManager::SetTransfer(bool Flag) {
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Active) {
				i->SetTransfer(Flag);
			}
		}
	}
}
AStation* AStationManager::GetNearestStationByType(int32 Start, StationType Type) {
	int32 StationNum = Station.Num();
	int32 NearestStationId = -1;
	float Distance = TNumericLimits<float>::Max();
	for (int i = 0; i < StationNum; i++) {
		if (GetStationById(i)->GetStationType() == Type) {
			if (AdjDist[Start][i] < Distance) {
				Distance = AdjDist[Start][i];
				NearestStationId = i;
			}
		}
	}
	return GetStationById(NearestStationId);
}

void AStationManager::FloydWarshall() {
	int32 StationNum = Station.Num();
	// Init dist
	for (int i = 0; i < StationNum; i++) {
		for (int j = 0; j < StationNum; j++) {
			if (i == j) {
				AdjDist[i][j] = 0;
				AdjPath[i][j] = -1;
			}else if (adj[i][j] != TNumericLimits<float>::Max()) {
				AdjDist[i][j] = adj[i][j];
				AdjPath[i][j] = j;
			}else { 
				AdjDist[i][j] = TNumericLimits<float>::Max();
				AdjPath[i][j] = -1;
			}
		}
	}
	

	// Floyd-Warshall
	
	for (int k = 0; k < StationNum; k++) {
		for (int i = 0; i < StationNum; i++) {
			for (int j = 0; j < StationNum; j++) {
				if (AdjDist[i][j] > AdjDist[i][k] + AdjDist[k][j]) {
					AdjDist[i][j] = AdjDist[i][k] + AdjDist[k][j];
					AdjPath[i][j] = AdjPath[i][k];
				}
			}
		}
	}

	// Calculate route
	for (int i = 0; i < StationNum; i++) {
		for (int j = 0; j < StationNum; j++) {
			if (ShortestPath.Find(i) == nullptr) {
				ShortestPath.Add(i);
			}
			ShortestPath[i].Emplace(
				StaticCast<StationType>(j),
				PathFinding(i, StaticCast<StationType>(j))
			);
		}
	}
	
}

PathQueue AStationManager::PathFinding(int32 Start, StationType Type) {
	auto NearestStation = GetNearestStationByType(Start, Type);
	if (!IsValid(NearestStation)) return PathQueue();

	int32 End = NearestStation->GetStationId();
	if (AdjPath[Start][End] == -1) return PathQueue();

	// Storing the path in a vector
	PathQueue Path;
	while (Start != End) {
		Start = AdjPath[Start][End];
		Path.Enqueue(Start);
	}

	return Path;
}

void AStationManager::PrintPath(int32 Start, StationType Type, TQueue<int32>* Path) {
}

StationType AStationManager::GetRandomStationType() {
	return StationSpawnTable[FMath::RandRange(0, StationSpawnRange)];
}

StationType AStationManager::StationTypeFromString(FString Str, bool& Success) {
	Success = true;
	StationType result;
	Str = Str.ToLower();
	if (Str == TEXT("circle")) {
		result = StationType::Circle;
	} else if (Str == TEXT("triangle")) {
		result = StationType::Triangle;
	} else if (Str == TEXT("rectangle")) {
		result = StationType::Rectangle;
	} else if (Str == TEXT("cross")) {
		result = StationType::Cross;
	} else if (Str == TEXT("rhombus")) {
		result = StationType::Rhombus;
	} else if (Str == TEXT("oval")) {
		result = StationType::Oval;
	} else if (Str == TEXT("diamond")) {
		result = StationType::Diamond;
	} else if (Str == TEXT("pentagon")) {
		result = StationType::Pentagon;
	} else if (Str == TEXT("star")) {
		result = StationType::Star;
	} else if (Str == TEXT("fan")) {
		result = StationType::Fan;
	} else {
		Success = false;
	}
	return result;
}

FString AStationManager::StationTypeToString(StationType Type, bool& Success) {
	Success = true;
	FString result;
	if (Type == StationType::Circle){
		result = TEXT("Circle");
	} else if (Type == StationType::Triangle) {
		result = TEXT("Triangle");
	} else if (Type == StationType::Rectangle) {
		result = TEXT("Rectangle");
	} else if (Type == StationType::Cross) {
		result = TEXT("Cross");
	} else if (Type == StationType::Rhombus) {
		result = TEXT("Rhombus");
	} else if (Type == StationType::Oval) {
		result = TEXT("Oval");
	} else if (Type == StationType::Diamond) {
		result = TEXT("Diamond");
	} else if (Type == StationType::Pentagon) {
		result = TEXT("Pentagon");
	} else if (Type == StationType::Star) {
		result = TEXT("Star");
	} else if (Type == StationType::Fan) {
		result = TEXT("Fan");
	} else {
		Success = false;
	}
	return result;
}

void AStationManager::Save() {
	UStationManagerSaveGame* tmp = Cast<UStationManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UStationManagerSaveGame::StaticClass()));
	tmp->NextStationId = NextStationId;
	tmp->IsPassengerSpawnEnable = IsPassengerSpawnEnable;
	tmp->StationSpawnDataArr = StationSpawnLog;
	
	SaveManagerRef->Save(tmp, SaveActorType::StationManager);
}

bool AStationManager::Load() {
	if (!GameMode) {
		GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	if (!SaveManagerRef) {
		SaveManagerRef = GameMode->GetSaveManager();
	}
	if (!TimerRef) {
		TimerRef = GameMode->GetTimer();
	}
	UStationManagerSaveGame* tmp = Cast<UStationManagerSaveGame>(SaveManagerRef->Load(SaveActorType::StationManager));
	
	if (!IsValid(tmp)) {
		return false;
	}

	NextStationId = tmp->NextStationId;
	IsPassengerSpawnEnable = tmp->IsPassengerSpawnEnable;
	for (auto& i : tmp->StationSpawnDataArr) {
		SpawnStation(i.GridCellData, i.Type, i.StationId);
		
	}

	return true;
}

void AStationManager::EventEnd() {
	AdditionalPassengerSpawnProbabilityByEvent = 0.0f;
	FreePassengerSpawnProbabilityByEvent = 0.0f;
	ComplainIncreaseRateByEvent = 0.0f;

	for (auto& i : PassengerDestinationTypeWeight) {
		i.Value = 1.0f;
	}
}

void AStationManager::UpdatePolicy() {
	auto policyData = PolicyRef->GetPolicyData();
	FreePassengerSpawnProbabilityByPolicy = 0.0f;
	AdditionalPassengerSpawnProbabilityByPolicy = 0.0f;
	ComplainIncreaseRateByPolicy = 0.0f;


	if (policyData.PrioritySeat) {
		ComplainIncreaseRateByPolicy -= 0.05f;
	}

	if (policyData.HasCCTV) {
		ComplainIncreaseRateByPolicy -= 0.1f;
	}

	if (policyData.HasElevator) {
		ComplainIncreaseRateByPolicy -= 0.15f;
	}

	if (policyData.PrioritySeat) {
		FreePassengerSpawnProbabilityByPolicy += 0.2f;
	}

	if (policyData.HasBicycle) {
		AdditionalPassengerSpawnProbabilityByPolicy += 0.1f;
		ComplainIncreaseRateByPolicy += 0.1f;
	}

	if (policyData.HasTransfer) {
		ComplainIncreaseRateByPolicy -= 0.2f;
	}
}

// Called every frame
void AStationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StationSpawnRoutine(DeltaTime);

	float averageComplain = 0.0f;
	int32 serviceStationCount = 0;

	for (auto& i : Station) {
		if (i->GetStationInfo().IsActive) serviceStationCount++;
		averageComplain += i->GetComplain();
	}
	
	StatisticsManagerRef->DefaultStatistics.AverageComplain = averageComplain;
	StatisticsManagerRef->DefaultStatistics.ServiceStationCount = serviceStationCount;

}

