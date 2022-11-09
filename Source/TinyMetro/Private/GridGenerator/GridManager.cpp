// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGenerator/GridManager.h"

// Sets default values
AGridManager::AGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridManager::BeginPlay()
{
	Super::BeginPlay();
	
}

float AGridManager::GetGridCellSize() const {
	return GridCellSize;
}

void AGridManager::SetGridCellData(TArray<FGridCellData>& data) {
	GridCellData = data;
}

TArray<FGridCellData> AGridManager::GetGridCellData() const {
	// // O: insert return statement here
	return GridCellData;
}

