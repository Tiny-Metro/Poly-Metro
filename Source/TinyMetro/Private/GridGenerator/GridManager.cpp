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

void AGridManager::SetGridSize(FIntVector2& GridSize) {
	this->GridSize.X = GridSize.X;
	this->GridSize.Y = GridSize.Y;
}

TArray<FGridCellData> AGridManager::GetGridCellData() const {
	// // O: insert return statement here
	return GridCellData;
}

FGridCellData AGridManager::GetGridCellDataAtPoint(int X, int Y) const {
	if (X > GridSize.X || Y > GridSize.Y) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("AGridManager::GetGridCellDataAtPoin : Invalid Grid Index")));

		return FGridCellData();
	}
	// GridIndex = (GridCountX * Y) + X


	return GridCellData[(GridSize.X * Y) + X];
}

bool AGridManager::IsValidStationSpawn(int Coord) {
	return false;
}

