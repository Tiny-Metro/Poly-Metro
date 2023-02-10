// Fill out your copyright notice in the Description page of Project Settings.

#include "Misc/OutputDeviceNull.h"
#include "Lane/LaneManager.h"

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
	
}

// Called every frame
void ALaneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ALaneManager::CreatingNewLane(TArray<AStation*> SelectedStations) {

	// Load BP Class
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/TrackLine/BP_Lane.BP_Lane'")));

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
	tmpLane->LaneNum = NextLaneId;

	

	UE_LOG(LogTemp, Warning, TEXT("SelectedStations[%d]"), SelectedStations.Num());
	for (int i = 0; i < SelectedStations.Num(); i++) {
		
		if (IsValid(SelectedStations[i])) {
			SelectedStations[i]->SetLanes(NextLaneId);
			tmpLane->StationPoint.Add(SelectedStations[i]->GetCurrentGridCellData().WorldCoordination);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SelectedStations[%d] is null"), i);
		}
		
	}

	tmpLane->InitializeNewLane();


	Lanes.Add(tmpLane);

	UE_LOG(LogTemp, Warning, TEXT("StationPoint Num : %d"), tmpLane->StationPoint.Num());

	NextLaneId++;
}


void ALaneManager::AddStationInLane(int CurrentLane) {

	ALane* tmp = GetLane(CurrentLane);

	tmp->ExtendLane();
}

void ALaneManager::AddLane(ALane* Obj) {

}

ALane* ALaneManager::GetLane(int32 LaneId) {
	return Lanes[LaneId - 1];
}




