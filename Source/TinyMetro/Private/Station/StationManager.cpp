// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationManager.h"
#include "Lane/Lane.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationInfo.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

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

	//PlayerState = GetWorld()->GetControllerIterator()->Get()->GetPlayerState<ATinyMetroPlayerState>();
	//PlayerState = Cast<ATinyMetroPlayerState>(GetWorld()->GetPawnIterator()->Get()->GetPlayerState());
	PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	//Policy = Cast<APolicy>(UGameplayStatics::GetActorOfClass(GetWorld(), APolicy::StaticClass()));
	Policy = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetPolicy();

	//MaxStationCount = GridManager->GetGridSize()
	auto GridSize = GridManager->GetGridSize();
	int32 SpawnPrevent = GridManager->GetStationSpawnPrevent();


	if (IsValid(PlayerState)) {
		UE_LOG(LogTemp, Log, TEXT("PlayerState Valid"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("PlayerState Invalid"));
	}
	if (IsValid(Policy)) {
		UE_LOG(LogTemp, Log, TEXT("Policy Valid"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("Policy Invalid"));
	}


	StationSpawnRoutine();
	PolicyMaintenanceRoutine();



	/*FIntPoint test(3,3);
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.0f,
		FColor::Yellow,
		FString::Printf(TEXT("TEST : %d"), test.Size()));*/

	

	// Test
	/*FTimerHandle TestHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TestHandle,
		FTimerDelegate::CreateLambda([&]() {
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.0f,
					FColor::Yellow,
					FString::Printf(TEXT("Time : %f"), GetWorld()->GetTimeSeconds()));
		}),
		1.0f,
		true,
		1.0f
	);*/
	
	// Spawn default 3 stationsstat
	// Get GameMode, Get coord and station type
	//GameMode = (ATinyMetroGameModeBase*)GetWorld()->GetAuthGameMode();
	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	InitData = GameMode->GetInitData();

	for (auto& i : InitData) {
		SpawnStation(GridManager->GetGridCellDataByPoint(i.Key.X, i.Key.Y), i.Value, true);
	}

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Data : %d"), InitData.Num()));*/

	// Init adj matrix
	FAdjArray adjTmp;
	adjTmp.Init(TNumericLimits<float>::Max(), 301);
	adj.Init(adjTmp, 301);

	FPath pathTmp;
	pathTmp.Init(-1, 301);
	AdjPath.Init(pathTmp, 301);

	AdjDist.Init(adjTmp, 301);

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

AStation* AStationManager::GetNearestStation(FVector CurrentLocation, class ALane* LaneRef) {
	double Distance = FVector::Dist(CurrentLocation, Station[0]->GetCurrentGridCellData().WorldLocation);
	int StationIndex = 0;
	bool LaneValid = IsValid(LaneRef);
	//UE_LOG(LogTemp, Log, TEXT("Lane valid : %d"), LaneValid);
	for (int i = 1; i < Station.Num(); i++) {
		if (LaneValid && !Station[i]->GetLanes().FindByKey<int32>(LaneRef->GetLaneId())) continue;
		double tmp = FVector::Dist(CurrentLocation, Station[i]->GetCurrentGridCellData().WorldLocation);
		if (Distance > tmp) {
			Distance = tmp;
			StationIndex = i;
		}
	}
	/*FVector Result = Station[0]->GetCurrentGridCellData().WorldLocation;
	for (auto& i : Station) {
		double tmp = FVector::Dist(CurrentLocation, i->GetCurrentGridCellData().WorldLocation);
		if (Distance > tmp) {
			Distance = tmp;
			Result = i->GetCurrentGridCellData().WorldLocation;
		}
	}*/
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
	tmp->SetStationId(StationId);
	tmp->SetPolicy(Policy);

	tmp->SetStationInfo(StationId++, Type);

	if (ActivateFlag) {
		tmp->ActivateStation();
	}
	tmp->FinishSpawning(SpawnTransform);

	if (Policy->GetHasBicycle())
	{
		tmp->AddPassengerSpawnProbability(0.1, -1);
	}

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

	/*
	AdjList.Add(tmp->GetItem(), NewObject<UAdjArrayItem>());
	AdjList[tmp->GetItem()].Add(tmp2->GetItem(), Length);
	AdjList[tmp->GetItem()][tmp2->GetItem()] == Length;
	*/

	UE_LOG(LogTemp, Warning, TEXT("StationSpawn GridCellData intpoint: %d / %d"), GridCellData.WorldCoordination.X, GridCellData.WorldCoordination.Y);
	UE_LOG(LogTemp, Warning, TEXT("StationSpawn"));

	//Log
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Magenta,
			FString::Printf(TEXT("Stations : %d"), Station.Num()));*/
}

void AStationManager::StationSpawnRoutine() {
	/*GetWorld()->GetTimerManager().SetTimer(
		TimerSpawnStation,
		this, 
		&AStationManager::IncreaseSpawnParameter, 
		1.0f, 
		true, 
		1.0f);*/

	// Spawn loop
	GetWorld()->GetTimerManager().SetTimer(
		TimerSpawnStation,
		FTimerDelegate::CreateLambda([&]() {
			StationSpawnCurrent += StationSpawnPerSec;
			if (StationSpawnCurrent >= StationSpawnRequire) {
				SpawnStation(GridManager->GetGridCellDataRandom(), GetRandomStationType());

				/*if (GEngine)
					GEngine->AddOnScreenDebugMessage(
						-1,
						15.0f,
						FColor::Yellow,
						FString::Printf(TEXT("Spawn!")));*/
				StationSpawnCurrent = 0.0f;
			}

			//Log
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(
			//		-1,
			//		15.0f,
			//		FColor::Yellow,
			//		FString::Printf(TEXT("%d"), StationSpawnCurrent));
		}),
		1.0f,
		true,
		1.0f
	);

	
}

void AStationManager::PolicyMaintenanceRoutine() {
	GetWorld()->GetTimerManager().SetTimer(
		PolicyTimerStation,
		FTimerDelegate::CreateLambda([&]() {
			PolicyCostCurrent += PolicyCostPerSec;
			if (PolicyCostCurrent >= PolicyCostRequire) {
				
				int32 MaintenanceCost = Policy->GetCostForCCTV() + Policy->GetCostForElevator() + Policy->GetCostForServiceLevel();

				PlayerState->AddMoney(-(MaintenanceCost));

				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(
						-1,
						15.0f,
						FColor::Blue,
						FString::Printf(TEXT("MaintenanceCost : %d"), MaintenanceCost));
						
				}
				PolicyCostCurrent = 0.0f;
			}
		}),
		1.0f,
		true,
		1.0f
	);
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

	//AdjDist[Start->GetStationInfo().Id][End->GetStationInfo().Id] = Length;
	//AdjDist[Start->GetStationInfo().Id][End->GetStationInfo().Id] = Start->GetStationInfo().Id;

	UE_LOG(LogTemp, Warning, TEXT("AddList: StartId : %d / EndId : %d / Length : %f"), Start->GetStationId(), End->GetStationId(), (*AdjList)[End->GetStationInfo()][Start->GetStationInfo()]);
	UE_LOG(LogTemp, Warning, TEXT("AddList: StartId : %d / EndId : %d / Length : %f"), End->GetStationId(), Start->GetStationId(), (*AdjList)[Start->GetStationInfo()][End->GetStationInfo()]);

	
}

void AStationManager::RemoveAdjListItem(FIntPoint First,FIntPoint Second)
{
	AStation* Start = GetStationByGridCellData(First);
	AStation* End = GetStationByGridCellData(Second);

	(*AdjList)[Start->GetStationInfo()].RemoveRef(End->GetStationInfo());
	(*AdjList)[End->GetStationInfo()].RemoveRef(Start->GetStationInfo());

	FloydWarshall();

	adj[Start->GetStationInfo().Id][End->GetStationInfo().Id] = TNumericLimits<float>::Max();
	adj[End->GetStationInfo().Id][Start->GetStationInfo().Id] = TNumericLimits<float>::Max();


	UE_LOG(LogTemp, Warning, TEXT(" Remove AddList: StartId : %d / EndId : %d / Length : %d"), Start->GetStationId(), End->GetStationId(), (*AdjList)[Start->GetStationInfo()].Num());

}

AStation* AStationManager::GetStationByGridCellData(FIntPoint _IntPoint)
{
	for (int i = 0; i < Station.Num(); i++) {

		FIntPoint Coor = Station[i]->GetCurrentGridCellData().WorldCoordination;

		if ( Coor == _IntPoint) {
			return Station[i];
		}

		UE_LOG(LogTemp, Warning, TEXT("Get Station By GridCellData / Station : %d, %d / IntPoint : %d, %d"), Coor.X, Coor.Y, _IntPoint.X, _IntPoint.Y);
	}

	return nullptr;
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

AStation* AStationManager::GetNearestStationByType(int32 Start, StationType Type) {
	int32 NearestStationId;
	float Distance = TNumericLimits<float>::Max();
	for (int i = 0; i < 300; i++) {
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
	// Init dist
	for (int i = 0; i < 300; i++) {
		for (int j = 0; j < 300; j++) {
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
	
	for (int k = 0; k < 300; k++) {
		for (int i = 0; i < 300; i++) {
			for (int j = 0; j < 300; j++) {
				if (AdjDist[i][j] > AdjDist[i][k] + AdjDist[k][j]) {
					AdjDist[i][j] = AdjDist[i][k] + AdjDist[k][j];
					AdjPath[i][j] = AdjPath[i][k];
				}
			}
		}
	}
	
}

void AStationManager::PathFinding(int32 Start, StationType Type) {
	auto NearestStation = GetNearestStationByType(Start, Type);
	int32 End = NearestStation->GetStationId();
	if (AdjPath[Start][End] == -1)
		return;

	// Storing the path in a vector
	TQueue<int> Path;
	//vector<int> path = { u };
	while (Start != End) {
		Start = AdjPath[Start][End];
		Path.Enqueue(Start);
	}

}

StationType AStationManager::GetRandomStationType() {
	return StationSpawnTable[FMath::RandRange(0, StationSpawnRange)];
}

// Called every frame
void AStationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (Policy == nullptr) GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT(":("));
	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.0f, 
			FColor::Yellow, 
			FString::Printf(TEXT("%d : %f"), GetWorld()->TimeSeconds, DeltaTime));*/

}

