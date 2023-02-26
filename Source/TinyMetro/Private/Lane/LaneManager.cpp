// Fill out your copyright notice in the Description page of Project Settings.

#include "Lane/LaneManager.h"
#include "Misc/OutputDeviceNull.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
ALaneManager::ALaneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALaneManager::BeginPlay()
{
	Super::BeginPlay();

	StationManagerRef = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));
	
}

// Called every frame
void ALaneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALaneManager::SetCanAssginBridge(bool AssginBridge)
{
	CanAssignBridge = AssginBridge;
}

void ALaneManager::SetCanAssginTunnel(bool AssginTunnel)
{
	CanAssignTunnel = AssginTunnel;
}

void ALaneManager::AddNextLaneNums(int32 LaneNum)
{
	NextLaneNums.Add(LaneNum);

	NextLaneNums.Sort();
}

void ALaneManager::RemoveNextLaneNums()
{
	NextLaneNums.RemoveAt(0);
}

void ALaneManager::RemoveDestroyedLane(int LaneNum)
{
	Lanes.Remove(LaneNum);
}

void ALaneManager::CreatingNewLane(TArray<AStation*> SelectedStations) {

	// Load BP Class
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Lane/BP_Lane.BP_Lane'")));

	// Cast to BP
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	// Check object validation
	if (!SpawnActor) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN / Lane")));
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
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ALane* tmpLane = Cast<ALane>(GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnParams));

	

	//ALane* tmpLane = GetWorld()->SpawnActor<>();
	tmpLane->SetLaneId(NextLaneNums[0]);

	UE_LOG(LogTemp, Warning, TEXT("GetActorNameOrLabel : %s"), *(tmpLane->GetActorNameOrLabel()));

	

	UE_LOG(LogTemp, Warning, TEXT("SelectedStations[%d]"), SelectedStations.Num());
	for (int i = 0; i < SelectedStations.Num(); i++) {
		
		if (IsValid(SelectedStations[i])) {
			SelectedStations[i]->SetLanes(NextLaneNums[0]);
			FIntPoint Coor = SelectedStations[i]->GetCurrentGridCellData().WorldCoordination;
			tmpLane->StationPoint.Add(Coor);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SelectedStations[%d] is null"), i);
			return;
		}
		
	}

	FIntPoint Start = SelectedStations[0]->GetCurrentGridCellData().WorldCoordination;
	FIntPoint End = SelectedStations[1]->GetCurrentGridCellData().WorldCoordination;
	UE_LOG(LogTemp, Warning, TEXT("CreatingNewLane /IntPoint Start : %d, %d"), Start.X, Start.Y);
	UE_LOG(LogTemp, Warning, TEXT("CreatingNewLane /IntPoint End : %d, %d"), End.X, End.Y);


	tmpLane->AddAdjListDistance(Start, End, SelectedStations[0], SelectedStations[1]);

	tmpLane->InitializeNewLane();

	Lanes.Add(NextLaneNums[0], tmpLane);
	//Lanes.Add(tmpLane);

	UE_LOG(LogTemp, Warning, TEXT("StationPoint Num : %d"), tmpLane->StationPoint.Num());

	UE_LOG(LogTemp, Warning, TEXT("New LaneNum : %d"), NextLaneNums[0]);

	RemoveNextLaneNums();

	//StationManagerRef->AddAdjListItem(SelectedStations[0], SelectedStations[1], )
}


void ALaneManager::AddStationInLane(int CurrentLane) {

	ALane* tmp = GetLaneById(CurrentLane);

	tmp->ExtendLane();
}

void ALaneManager::AddLane(ALane* Obj) {

}

ALane* ALaneManager::GetLaneById(int32 LaneId) {

	ALane* Lane = Lanes.FindRef(LaneId);
	return Lane;
}
