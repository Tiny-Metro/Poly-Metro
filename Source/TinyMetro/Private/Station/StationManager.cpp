// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationManager.h"
#include "Lane/Lane.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationInfo.h"
#include "Timer/Timer.h"
#include "Station/StationInfoWidget.h"
#include "Station/StationSpawnBorderWidget.h"
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
}

// Called when the game starts or when spawned
void AStationManager::BeginPlay()
{
	Super::BeginPlay();

	// Init default data
	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	PolicyRef = GameMode->GetPolicy();

	auto GridSize = GridManager->GetGridSize();
	int32 SpawnPrevent = GridManager->GetStationSpawnPrevent();


	if (IsValid(PlayerState)) {
		UE_LOG(LogTemp, Log, TEXT("PlayerState Valid"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("PlayerState Invalid"));
	}
	if (IsValid(PolicyRef)) {
		UE_LOG(LogTemp, Log, TEXT("Policy Valid"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("Policy Invalid"));
	}

	// Init info widget
	InitStationInfoWidget();

	// Init spawn alarm widget
	InitStationSpawnWidget();

	// Load initializa station data
	// Need to modify by SaveManager
	InitData = GameMode->GetInitData();

	for (auto& i : InitData) {
		SpawnStation(GridManager->GetGridCellDataByPoint(i.Key.X, i.Key.Y), i.Value, true);
	}


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

	// Test Spawn actors
	/*for (int i = 0; i < 300; i++) {
		GetWorld()->SpawnActor<AStation>();
	}*/

	// Register Timer tasks
	GameMode->GetTimer()->DailyTask.AddDynamic(this, &AStationManager::DailyTask);
	GameMode->GetTimer()->WeeklyTask.AddDynamic(this, &AStationManager::WeeklyTask);

	if (IsValid(StationInfoWidget)) {
		UE_LOG(LogTemp, Warning, TEXT("StationInfoWidget Valid"));
	} else {
		UE_LOG(LogTemp, Warning, TEXT("StationInfoWidget Invalid"));
	}

}

void AStationManager::TestFunction() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT(":)")));
}

StationType AStationManager::CalculatePassengerDest(StationType Except) const {
	StationType tmp;

	do {
		tmp = Station[FMath::RandRange(0, Station.Num()-1)]->GetStationType();
	} while (tmp == Except);

	return tmp;
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

void AStationManager::SpawnStation(FGridCellData GridCellData, StationType Type, bool ActivateFlag = false) {
	
	// Load BP Class
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Station/BP_Station.BP_Station'")));

	// Cast to BP
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	// Check object validation
	if (!SpawnActor) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}

	// Check null
	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return;
	}

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(GridCellData.WorldLocation);
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//AStation* tmp = Cast<AStation>(GetWorld()->SpawnActorDeferred<AActor>(GeneratedBP->GeneratedClass, GridCellData.WorldLocation, GetActorRotation(), SpawnParams));
	AStation* tmp = Cast<AStation>(GetWorld()->SpawnActorDeferred<AActor>(GeneratedBP->GeneratedClass, SpawnTransform));
	tmp->SetStationType(Type);
	tmp->SetGridCellData(GridCellData);
	tmp->SetStationId(NextStationId);

	tmp->SetStationInfo(NextStationId++, Type);
	tmp->SetPassengerSpawnEnable(IsPassengerSpawnEnable);
	tmp->SetInfoWidget(StationInfoWidget);

	if (ActivateFlag) {
		tmp->SetActivate(true);
	}
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


	AdjList->Add(tmp->GetStationInfo(), NewObject<UAdjArrayItem>());

	UE_LOG(LogTemp, Warning, TEXT("StationSpawn GridCellData intpoint: %d / %d"), GridCellData.WorldCoordination.X, GridCellData.WorldCoordination.Y);
	UE_LOG(LogTemp, Warning, TEXT("StationSpawn"));

	// For test : camera location
	auto tmpPawnLocation = UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetPawn()->GetActorLocation();
	auto tmpStationLocation = SpawnTransform.GetLocation();
	UE_LOG(LogTemp, Warning, TEXT("StationSpawn::PawnLocation : %f, %f, %f"), tmpPawnLocation.X, tmpPawnLocation.Y, tmpPawnLocation.Z);

	// Decide spawn direction
	auto spawnDirection = tmpPawnLocation - tmpStationLocation;
	if (abs(spawnDirection.X) >= abs(spawnDirection.Y)) {
		// Spawn X direction (Left or Right)
		if (spawnDirection.X > 0) {
			// Spawn Right
			UE_LOG(LogTemp, Warning, TEXT("StationSpawn::Spawn Left"));
			StationSpawnWidget->AlarmLeft();
		} else {
			// Spawn Left
			UE_LOG(LogTemp, Warning, TEXT("StationSpawn::Spawn Right"));
			StationSpawnWidget->AlarmRight();
		}
	} else {
		// Spawn Y direction (Top or Bottom)
		if (spawnDirection.Y > 0) {
			// Spawn Bottom
			UE_LOG(LogTemp, Warning, TEXT("StationSpawn::Spawn Top"));
			StationSpawnWidget->AlarmTop();
		} else {
			// Spawn Top
			UE_LOG(LogTemp, Warning, TEXT("StationSpawn::Spawn Bottom"));
			StationSpawnWidget->AlarmBottom();
		}
	}

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

void AStationManager::PolicyMaintenanceRoutine() {

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

PathQueue AStationManager::GetShortestPath(int32 Start, StationType Type) {
	if (ShortestPath.Find(Start) == nullptr) {
		return PathQueue();
	}
	if (ShortestPath[Start].Find(Type) == nullptr) {
		return PathQueue();
	}
	return ShortestPath[Start][Type];
}

void AStationManager::PassengerPropertyTick(float DeltaTime) {
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

		// Calc multiple factor
		for (auto& i : MultiplePassengerSpawnSpeedArr[Type]) {
			multiFactor *= i.Key;
		}

		// Calc add factor
		for (auto& i : AddPassengerSpawnSpeedArr[Type]) {
			addFactor += i.Key;
		}

		return PassengerSpawnSpeed[Type] * multiFactor + addFactor;
	}
}

void AStationManager::AddComplainIncreaseRate(float Rate, int32 Period) {
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Active) {
				i->AddComplainIncreaseRate(Rate, Period);
			}
		}
	}
}

void AStationManager::SetServiceData(FServiceData _ServiceData) {
	ServiceData = _ServiceData;
}

void AStationManager::NotifySpawnPassenger(StationType Type, bool IsFree) {
	if (IsFree) {
		TotalSpawnPassengerFree[Type]++;
	} else {
		TotalSpawnPassengerNotFree[Type]++;
	}
	TotalSpawnPassenger[Type]++;
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

int32 AStationManager::GetTotalPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		auto passenger = i->GetSpawnTotalPassenger();
		if (Type == StationType::None) {
			for (auto& j : passenger) {
				result += j.Value;
			}
		} else {
			result += passenger[Type];
		}
	}
	return result;
}

int32 AStationManager::GetFreePassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		auto passenger = i->GetSpawnFreePassenger();
		if (Type == StationType::None) {
			for (auto& j : passenger) {
				result += j.Value;
			}
		} else {
			result += passenger[Type];
		}
	}
	return result;
}

int32 AStationManager::GetPaidPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		auto passenger = i->GetSpawnPaidPassenger();
		if (Type == StationType::None) {
			for (auto& j : passenger) {
				result += j.Value;
			}
		} else {
			result += passenger[Type];
		}
	}
	return result;
}

int32 AStationManager::GetArriveTotalPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (Type == StationType::None) {
			result += i->GetArriveTotalPassengerCount();
		} else {
			if (i->GetStationType() == Type) {
				result += i->GetArriveTotalPassengerCount();
			}
		}
	}
	return result;
}

int32 AStationManager::GetArriveFreePassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (Type == StationType::None) {
			result += i->GetArriveFreePassengerCount();
		} else {
			if (i->GetStationType() == Type) {
				result += i->GetArriveFreePassengerCount();
			}
		}
	}
	return result;
}

int32 AStationManager::GetArrivePaidPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (Type == StationType::None) {
			result += i->GetArrivePaidPassengerCount();
		} else {
			if (i->GetStationType() == Type) {
				result += i->GetArrivePaidPassengerCount();
			}
		}
	}
	return result;
}

int32 AStationManager::GetWaitTotalPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (Type == StationType::None) {
			result += i->GetWaitTotalPassengerCount();
		} else {
			if (i->GetStationType() == Type) {
				result += i->GetWaitTotalPassengerCount();
			}
		}
	}
	return result;
}

int32 AStationManager::GetWaitFreePassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (Type == StationType::None) {
			result += i->GetWaitFreePassengerCount();
		} else {
			if (i->GetStationType() == Type) {
				result += i->GetWaitFreePassengerCount();
			}
		}
	}
	return result;
}

int32 AStationManager::GetWaitPaidPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (Type == StationType::None) {
			result += i->GetWaitPaidPassengerCount();
		} else {
			if (i->GetStationType() == Type) {
				result += i->GetWaitPaidPassengerCount();
			}
		}
	}
	return result;
}

int32 AStationManager::GetDestroyedTotalPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		auto passenger = i->GetDestroyedTotalPassenger();
		if (Type == StationType::None) {
			for (auto& j : passenger) {
				result += j.Value;
			}
		} else {
			result += passenger[Type];
		}
	}
	return result;
}

int32 AStationManager::GetDestroyedFreePassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		auto passenger = i->GetDestroyedFreePassenger();
		if (Type == StationType::None) {
			for (auto& j : passenger) {
				result += j.Value;
			}
		} else {
			result += passenger[Type];
		}
	}
	return result;
}

int32 AStationManager::GetDestroyedPaidPassengerCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		auto passenger = i->GetDestroyedPaidPassenger();
		if (Type == StationType::None) {
			for (auto& j : passenger) {
				result += j.Value;
			}
		} else {
			result += passenger[Type];
		}
	}
	return result;
}

int32 AStationManager::GetRideTotalPassengerCount(StationType Type) const {
	return GetTotalPassengerCount(Type) 
		- GetArriveTotalPassengerCount(Type)
		- GetWaitTotalPassengerCount(Type)
		- GetDestroyedTotalPassengerCount(Type);
}

int32 AStationManager::GetRideFreePassengerCount(StationType Type) const {
	return GetFreePassengerCount(Type)
		- GetArriveFreePassengerCount(Type)
		- GetWaitFreePassengerCount(Type)
		- GetDestroyedFreePassengerCount(Type);
}

int32 AStationManager::GetRidePaidPassengerCount(StationType Type) const {
	return GetPaidPassengerCount(Type)
		- GetArrivePaidPassengerCount(Type)
		- GetWaitPaidPassengerCount(Type)
		- GetDestroyedPaidPassengerCount(Type);
}

TMap<StationType, int32> AStationManager::GetSpawnPassengerStatistics(int32& TotalPassenger, int32& WaitPassenger, int32 SID) {
	TMap<StationType, int32> result;
	TotalPassenger = 0;
	WaitPassenger = 0;
	if (SID == -1) {
		result = TotalSpawnPassenger;
		for (auto& i : Station) {
			WaitPassenger += i->GetWaitPassenger();
		}
	} else {
		auto station = GetStationById(SID);
		if (IsValid(station)) {
			result = station->GetSpawnPassengerStatistics();
			WaitPassenger = station->GetWaitPassenger();
		} else {
			WaitPassenger = -1;
			UE_LOG(LogTemp, Error, TEXT("Invalid station ID"));
		}
	}
	
	for (auto& i : result) {
		TotalPassenger += i.Value;
	}

	return result;
}

void AStationManager::WeeklyTask() {
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Active) {
				// Add weekly tasks
				
				// From policy (Cost)
				PlayerState->AddMoney(PolicyRef->GetTotalCost());
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
				// Add weekly tasks

				// From service level (Complain)
				i->AddComplain(-ServiceData.DailyComplain);
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

int32 AStationManager::GetStationCount() const {
	return Station.Num();
}

int32 AStationManager::GetActiveStationCount() const {
	int32 result = 0;
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Active) result++;
		}
	}
	return result;
}

int32 AStationManager::GetInactiveStationCount() const {
	int32 result = 0;
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Inactive) result++;
		}
	}
	return result;
}

int32 AStationManager::GetDestroyedStationCount() const {
	int32 result = 0;
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationState() == StationState::Destroyed) result++;
		}
	}
	return result;
}

int32 AStationManager::GetTypeStationCount(StationType Type) const {
	int32 result = 0;
	for (auto& i : Station) {
		if (IsValid(i)) {
			if (i->GetStationType() == Type) result++;
		}
	}
	return result;
}

FStationInfo AStationManager::GetRandomStationInfo() const {
	int32 randomIndex = FMath::RandRange(0, Station.Num() - 1);
	do {
		randomIndex = FMath::RandRange(0, Station.Num() - 1);
	} while (!Station.IsValidIndex(randomIndex) && !IsValid(Station[randomIndex]));
	return Station[randomIndex]->GetStationInfo();
}

FStationInfo AStationManager::GetRandomActiveStationInfo() const {
	int32 randomIndex = FMath::RandRange(0, Station.Num() - 1);
	do {
		randomIndex = FMath::RandRange(0, Station.Num() - 1);
	} while ((!Station.IsValidIndex(randomIndex) && !IsValid(Station[randomIndex]))
		&& Station[randomIndex]->GetStationState() == StationState::Active);
	return Station[randomIndex]->GetStationInfo();
}

FStationInfo AStationManager::GetRandomInactiveStationInfo() const {
	int32 randomIndex = FMath::RandRange(0, Station.Num() - 1);
	do {
		randomIndex = FMath::RandRange(0, Station.Num() - 1);
	} while ((!Station.IsValidIndex(randomIndex) && !IsValid(Station[randomIndex]))
		&& Station[randomIndex]->GetStationState() == StationState::Inactive);
	return Station[randomIndex]->GetStationInfo();
}

FStationInfo AStationManager::GetRandomDestroyedStationInfo() const {
	int32 randomIndex = FMath::RandRange(0, Station.Num() - 1);
	do {
		randomIndex = FMath::RandRange(0, Station.Num() - 1);
	} while ((!Station.IsValidIndex(randomIndex) && !IsValid(Station[randomIndex]))
		&& Station[randomIndex]->GetStationState() == StationState::Destroyed);
	return Station[randomIndex]->GetStationInfo();
}

FStationInfo AStationManager::GetRandomTypeStationInfo(StationType Type) const {
	int32 randomIndex = FMath::RandRange(0, Station.Num() - 1);
	do {
		randomIndex = FMath::RandRange(0, Station.Num() - 1);
	} while ((!Station.IsValidIndex(randomIndex) && !IsValid(Station[randomIndex]))
		&& Station[randomIndex]->GetStationType() == Type);
	return Station[randomIndex]->GetStationInfo();
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
		for (int j = 0; j < 10; j++) {
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

// Called every frame
void AStationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StationSpawnRoutine(DeltaTime);

	//if (Policy == nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT(":("));
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.0f, 
			FColor::Yellow, 
			FString::Printf(TEXT("%d : %f"), GetWorld()->TimeSeconds, DeltaTime));*/

}

