// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainManager.h"

// Sets default values
ATrainManager::ATrainManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATrainManager::AddTrain(ATrainTemplate* Train) {
	Trains.Add(Train);
}

void ATrainManager::SetTrainId(ATrainTemplate* Train) {
	Train->SetTrainId(NextTrainId++);
}

ATrainTemplate* ATrainManager::GetTrainById(int32 TrainId) {
	for (auto i : Trains) {
		if (TrainId == i->GetTrainId()) {
			return i;
		}
	}
	return nullptr;
}

// Called every frame
void ATrainManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

