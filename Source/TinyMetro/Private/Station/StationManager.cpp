// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AStationManager::AStationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Init GridManager
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

}

// Called when the game starts or when spawned
void AStationManager::BeginPlay()
{
	Super::BeginPlay();

	StationSpawnRoutine();

	FIntPoint test(3,3);
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.0f,
		FColor::Yellow,
		FString::Printf(TEXT("TEST : %d"), test.Size()));

	

	// Test
	FTimerHandle TestHandle;
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
	);
	
	// Spawn default 3 stationsstat
	// Get GameMode, Get coord and station type
	GameMode = (ATinyMetroGameModeBase*)GetWorld()->GetAuthGameMode();

	InitData = GameMode->GetInitData();

	for (auto& i : InitData) {
		SpawnStation(GridManager->GetGridCellDataByPoint(i.Key.X, i.Key.Y), i.Value, true);
	}

	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Data : %d"), InitData.Num()));
}

void AStationManager::TestFunction() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT(":)")));
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
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStation* tmp = Cast<AStation>(GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, GridCellData.WorldLocation, GetActorRotation(), SpawnParams));
	tmp->SetStationType(Type);
	tmp->SetStationId(StationId++);
	if (ActivateFlag) {
		tmp->ActivateStation();
	}

	Station.Add(tmp);
	GridManager->SetGridStructure(
		GridCellData.WorldCoordination.X,
		GridCellData.WorldCoordination.Y, 
		GridStructure::Station);


	//Log
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Magenta,
			FString::Printf(TEXT("Stations : %d"), Station.Num()));
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

				if (GEngine)
					GEngine->AddOnScreenDebugMessage(
						-1,
						15.0f,
						FColor::Yellow,
						FString::Printf(TEXT("Spawn!")));
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

StationType AStationManager::GetRandomStationType() {
	return StationSpawnTable[FMath::RandRange(0, StationSpawnRange)];
}

// Called every frame
void AStationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.0f, 
			FColor::Yellow, 
			FString::Printf(TEXT("%d : %f"), GetWorld()->TimeSeconds, DeltaTime));*/

}

