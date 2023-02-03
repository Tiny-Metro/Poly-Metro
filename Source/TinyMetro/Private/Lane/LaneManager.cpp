// Fill out your copyright notice in the Description page of Project Settings.


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


void ALaneManager::CreatingNewLane(AStation* SelectedStation) {
	
}


void ALaneManager::AddStationInLane(ALane* CurrentLane , AStation* SelectedStation) {

}

void ALaneManager::AddLane(ALane* Obj) {

}

ALane* ALaneManager::GetLane(int32 LaneId) {
	return Lanes[LaneId - 1];
}

