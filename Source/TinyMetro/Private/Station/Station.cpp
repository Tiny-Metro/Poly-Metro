// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Station.h"
#include "Station/StationManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Engine/AssetManager.h>

// Sets default values
AStation::AStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Ser root
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	// Set station mesh
	StationMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Station Mesh"));
	StationMeshComponent->SetupAttachment(RootComponent);

	// Set passenger mesh
	for (int i = 0; i < MaxPassengerSpawn; i++) {
		FName name = *FString::Printf(TEXT("Passenger %d"), i);
		auto tmp = CreateDefaultSubobject<UStaticMeshComponent>(name);
		tmp->SetupAttachment(RootComponent);
		FVector PassengerPosition =
			PassengerMeshDefaultPosition +
			FVector(PassengerX_Distance * (i / 2), 0.0f, 0.0f) +
			FVector(0.0f, PassengerY_Distance * ((i % 2) == 0 ? 1 : -1), 0.0f);
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

}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{

	Super::BeginPlay();
	
	// Get GameMode, set daytime
	ATinyMetroGameModeBase* GameMode = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	StationManager = GameMode->GetStationManager();
	Daytime = GameMode->GetDaytime();

	PassengerSpawnRoutine();
	ComplainRoutine();

	StationMeshComponent->SetStaticMesh(StationMesh[(int)StationTypeValue]);
	UpdateStationMesh();
	UpdatePassengerMesh();

	// Log ( I am {Actor Name} )
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,
			FString::Printf(TEXT("I am %s"), *this->GetActorLabel()));
	}*/


}

// Called every frame
void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStation::SetStationId(int32 Id) {
	StationId = Id;
}

int32 AStation::GetStationId() const {
	return StationId;
}

void AStation::SetStationType(StationType Type) {
	StationTypeValue = Type;

	//LOG
	/*FString EnumToStr = TEXT("NULL");
	const UEnum* MyType = FindObject<UEnum>(ANY_PACKAGE, TEXT("StationType"), true);
	if (MyType) {
		EnumToStr = MyType->GetNameStringByValue((int64)StationTypeValue);
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,
			EnumToStr);
	}*/
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

void AStation::LoadStationValue(FStationValuesStruct StationValues) {
	ComplainCurrent = StationValues.ComplainCurrent;
	StationId = StationValues.StationId;
	if (StationValues.IsActive) {
		ActivateStation();
	};
	SetStationType(StationValues.StationTypeValue);
	SetGridCellData(StationValues.GridCellData);

	for (int i = 0; i < StationValues.passengers.Num(); i++) {
		FPassengerValues passengerValue = StationValues.passengers[i];

		//TODO 승객 로드 추가하기
		UPassenger* tmp = UPassenger::ConstructPassenger(passengerValue.Destination);
		Passenger.Add(tmp);
	}
}

void AStation::AddPassengerSpawnProbability(float rate, int32 dueDate) {
	AdditionalPassengerSpawnProbability += rate;
	if (dueDate != -1) {
		GetWorld()->GetTimerManager().SetTimer(
			TimerComplain,
			FTimerDelegate::CreateLambda([&]() {
				AdditionalPassengerSpawnProbability -= rate;
				}),
			dueDate,
					false,
					0.0f
					);
	}
}
  
bool AStation::IsValidLane(int32 LId) const
{
	return false;
}



void AStation::CalculateComplain() {
}

void AStation::ActivateStation() {
	IsActive = true;
	State = StationState::Active;
	
	if (IsValid(this)) {
		UpdateStationMesh();
	}

	// TODO :  Visible logic
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
	return StationTypeValue;
}

void AStation::DecreaseComplain(double ReduceRate) {
	ComplainCurrent /= ReduceRate;
}

void AStation::DecreaseComplain(int32 ReduceValue) {
	ComplainCurrent -= ReduceValue;
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

void AStation::ComplainRoutine() {
	GetWorld()->GetTimerManager().SetTimer(
		TimerComplain,
		FTimerDelegate::CreateLambda([&]() {
			SpawnDay++;

			int AddPolicyComplainForLevel = Policy->GetComplainForServiceLevel();

			float AddPolicyComplain = AddPolicyComplainForLevel + abs(AddPolicyComplainForLevel) * Policy->CalculateComplainPercentage();
			
			// Passenger complain
			if (Passenger.Num() > ComplainPassengerNum) {
				ComplainCurrent += (ComplainFromPassenger * (Passenger.Num() - ComplainPassengerNum)) + AddPolicyComplain;
			}

			// Not activate
			if (!IsActive && SpawnDay > ComplainSpawnDay) {
				ComplainCurrent += ComplainFromInactive + AddPolicyComplain;
			}

			/*
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.0f,
					FColor::Yellow,
					FString::Printf(TEXT("AddPolicyComplain : %f"), AddPolicyComplain));

			}*/



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
	for (int i = 0; i < MaxPassengerSpawn; i++) {
		if (Passenger.IsValidIndex(i)) {
			PassengerMeshComponent[i]->SetStaticMesh(PassengerMesh[(int)Passenger[i]->GetDestination()]);
		}
	}
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
		1.0f, // Repeat delay
		true, // Repeat
		1.0f // First delay
		);
}

void AStation::SpawnPassenger() {
	UPassenger* tmp = UPassenger::ConstructPassenger(
		StationManager->CalculatePassengerDest(StationTypeValue)
	);
	//UPassenger* tmp = NewObject<UPassenger>();
	//tmp->SetDestination(StationManager->CalculatePassengerDest(StationTypeValue));

	if (Policy->GetHandicappedSeat()) {
		tmp->SetFree();
	}

	Passenger.Add(MoveTemp(tmp));

	//Log
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Passenger Spawn!")));
}

double AStation::GetPassengerSpawnProbability() {
	
	return PassengerSpawnProbability * AdditionalPassengerSpawnProbability;
}


