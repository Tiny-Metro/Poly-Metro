// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Station.h"
#include "Station/StationManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AStation::AStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{
	Super::BeginPlay();
	
	// Get StationManager
	StationManager = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));

	// Get GameMode, set daytime
	ATinyMetroGameModeBase* GameMode = (ATinyMetroGameModeBase*)GetWorld()->GetAuthGameMode();
	Daytime = GameMode->GetDaytime();

	PassengerSpawnRoutine();
	ComplainRoutine();


	// Log
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,
			FString::Printf(TEXT("I am %s"), *this->GetActorLabel()));
	}


}

// Called every frame
void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStation::SetStationId(int32 Id) {
	StationId = Id;
}

void AStation::SetStationType(StationType Type) {
	StationTypeValue = Type;

	// Set station's mesh

	//LOG
	FString EnumToStr = TEXT("NULL");
	const UEnum* MyType = FindObject<UEnum>(ANY_PACKAGE, TEXT("StationType"), true);
	if (MyType) {
		EnumToStr = MyType->GetNameStringByValue((int64)StationTypeValue);
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,
			EnumToStr);
	}
}

void AStation::SetGridCellData(FGridCellData gridCellData) {

	GridCellData = gridCellData;
}

void AStation::SetComplainCurrent(int32 complaincurrent) {
	ComplainCurrent = complaincurrent;
}

void AStation::CalculateComplain() {
}

void AStation::ActivateStation() {
	IsActive = true;

	// TODO :  Visible logic
}

StationType AStation::GetStationType() {
	return StationTypeValue;
}

void AStation::DecreaseComplain(double ReduceRate) {
	ComplainCurrent /= ReduceRate;
}

void AStation::DecreaseComplain(int32 ReduceValue) {
	ComplainCurrent -= ReduceValue;
}

void AStation::ComplainRoutine() {
	GetWorld()->GetTimerManager().SetTimer(
		TimerComplain,
		FTimerDelegate::CreateLambda([&]() {
			SpawnDay++;
			// Passenger complain
			if (Passenger.Num() > ComplainPassengerNum) {
				ComplainCurrent += (ComplainFromPassenger * (Passenger.Num() - ComplainPassengerNum));
			}

			// Not activate
			if (!IsActive && SpawnDay > ComplainSpawnDay) {
				ComplainCurrent += ComplainFromInactive;
			}


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
		}),
		Daytime,
		true,
		Daytime
		);
}

void AStation::UpdatePassengerMesh() {
	// Read passenger array, clear and reorganize meshes
}

void AStation::PassengerSpawnRoutine() {
	GetWorld()->GetTimerManager().SetTimer(
		TimerSpawnPassenger,
		FTimerDelegate::CreateLambda([&]() {
			PassengerSpawnCurrent += PassengerSpawnPerSec;
			if (PassengerSpawnCurrent >= PassengerSpawnRequire) {
				if (FMath::RandRange(0.0, 1.0) > GetPassengerSpawnProbability()) {
					SpawnPassenger();
					UpdatePassengerMesh();
				}

				PassengerSpawnCurrent = 0.0f;
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

void AStation::SpawnPassenger() {
	UPassenger* tmp = UPassenger::ConstructPassenger(
		StationManager->CalculatePassengerDest(StationTypeValue)
	);
	//UPassenger* tmp = NewObject<UPassenger>();
	//tmp->SetDestination(StationManager->CalculatePassengerDest(StationTypeValue));
	Passenger.Add(tmp);

	//Log
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Passenger Spawn!")));
}

double AStation::GetPassengerSpawnProbability() {
	double temp = PassengerSpawnProbability;
	for (auto& i : PassengerSpawnProbabilityVariable) {
		temp *= i;
	}
	return temp;
}


