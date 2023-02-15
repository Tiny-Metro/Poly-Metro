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

	// Ser root
	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);

	// Set station mesh
	MeshInner = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Inner"));
	MeshOuter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Outer"));

	MeshInner->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	MeshOuter->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	
	// Load material
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialActiveInner(TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationActive_Inner.M_StationActive_Inner'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialActiveOuter(TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationActive_Outer.M_StationActive_Outer'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialInactiveInner(TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationInactive_Inner.M_StationInactive_Inner'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialInactiveOuter(TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationInactive_Outer.M_StationInactive_Outer'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialDestroyedInner(TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationDestroyed_Inner.M_StationDestroyed_Inner'"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialDestroyedOuter(TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationDestroyed_Outer.M_StationDestroyed_Outer'"));

	// Load Static Mesh
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundStaticMeshTempInner(TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCircle_Inner.SM_StationCircle_Inner'"));
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> FoundStaticMeshTempOuter(TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCircle_Outer.SM_StationCircle_Outer'"));

	MaterialActiveInner = FoundMaterialActiveInner.Object;
	MaterialActiveOuter = FoundMaterialActiveOuter.Object;
	MaterialInactiveInner = FoundMaterialInactiveInner.Object;
	MaterialInactiveOuter = FoundMaterialInactiveOuter.Object;
	MaterialDestroyedInner = FoundMaterialDestroyedInner.Object;
	MaterialDestroyedOuter = FoundMaterialDestroyedOuter.Object;

	DynamicMaterialInner = UMaterialInstanceDynamic::Create(MaterialInactiveInner, MeshInner);
	DynamicMaterialOuter = UMaterialInstanceDynamic::Create(MaterialInactiveOuter, MeshOuter);
	/*MeshInner->SetStaticMesh(FoundStaticMeshTempInner.Object);
	MeshOuter->SetStaticMesh(FoundStaticMeshTempOuter.Object);
	MeshInner->SetMaterial(0, DynamicMaterialInner);
	MeshOuter->SetMaterial(0, DynamicMaterialOuter);*/
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

	// Set station's mesh
	FString MeshInnerName;
	FString MeshOuterName;

	switch (Type) {
	case StationType::Almond:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationAlmond_Inner.SM_StationAlmond_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationAlmond_Outer.SM_StationAlmond_Outer'");
		break;
	case StationType::Circle:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCircle_Inner.SM_StationCircle_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCircle_Outer.SM_StationCircle_Outer'");
		break;
	case StationType::Cross:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCross_Inner.SM_StationCross_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCross_Outer.SM_StationCross_Outer'");
		break;
	case StationType::Diamond:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationDiamond_Inner.SM_StationDiamond_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationDiamond_Outer.SM_StationDiamond_Outer'");
		break;
	case StationType::Fan:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationFan_Inner.SM_StationFan_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationFan_Outer.SM_StationFan_Outer'");
		break;
	case StationType::Lozenge:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationLozenge_Inner.SM_StationLozenge_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationLozenge_Outer.SM_StationLozenge_Outer'");
		break;
	case StationType::Pentagon:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationPentagon_Inner.SM_StationPentagon_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationPentagon_Outer.SM_StationPentagon_Outer'");
		break;
	case StationType::Square:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationSquare_Inner.SM_StationSquare_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationSquare_Outer.SM_StationSquare_Outer'");
		break;
	case StationType::Star:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationStar_Inner.SM_StationStar_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationStar_Outer.SM_StationStar_Outer'");
		break;
	case StationType::Triangle:
		MeshInnerName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationTriangle_Inner.SM_StationTriangle_Inner'");
		MeshOuterName = TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationTriangle_Outer.SM_StationTriangle_Outer'");
		break;
	}

	UStaticMesh* LoadMeshInner = Cast<UStaticMesh>(
		StaticLoadObject(UStaticMesh::StaticClass(), NULL,
			*MeshInnerName)
		);
	UStaticMesh* LoadMeshOuter = Cast<UStaticMesh>(
		StaticLoadObject(UStaticMesh::StaticClass(), NULL,
			*MeshOuterName)
		);

	MeshInner->SetStaticMesh(LoadMeshInner);
	MeshOuter->SetStaticMesh(LoadMeshOuter); 
	MeshInner->SetWorldLocation(this->GetActorLocation());
	MeshOuter->SetWorldLocation(this->GetActorLocation());

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

void AStation::AddPassengerSpawnProbability(float rate, int32 dueDate){
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
  
bool AStation::IsValidLane(int32 LId) const
{
	return false;
}



void AStation::CalculateComplain() {
}

void AStation::ActivateStation() {
	IsActive = true;

	// TODO :  Visible logic
	DynamicMaterialInner = UMaterialInstanceDynamic::Create(MaterialActiveInner, MeshInner);
	DynamicMaterialOuter = UMaterialInstanceDynamic::Create(MaterialActiveOuter, MeshOuter);
	MeshInner->SetMaterial(0, DynamicMaterialInner);
	MeshOuter->SetMaterial(0, DynamicMaterialOuter);
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
	
	return PassengerSpawnProbability * AdditionalPassengerSpawnProbability;
}


