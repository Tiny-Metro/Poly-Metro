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

void AGridManager::SetGridSize(FIntPoint& Size) {
	GridSize = Size;
}

TArray<FGridCellData> AGridManager::GetGridCellData() const {
	// // O: insert return statement here
	return GridCellData;
}

FGridCellData AGridManager::GetGridCellDataByPoint(int X, int Y) const {
	if (X > GridSize.X || Y > GridSize.Y) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("AGridManager::GetGridCellDataAtPoin : Invalid Grid Index")));

		return FGridCellData();
	}
	// GridIndex = (GridCountX * Y) + X

	return GetGridCellDataByIndex((GridSize.X * Y) + X);
}

FGridCellData AGridManager::GetGridCellDataByIndex(int Index) const {
	if (Index > GridCellData.Num()) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.0f,
				FColor::Red,
				FString::Printf(TEXT("GetGridCellDataByIndex : Out of range")));
		return FGridCellData();
	}
	return GridCellData[Index];
}

FGridCellData AGridManager::GetGridCellDataRandom() const {
	return FGridCellData();
}

void AGridManager::SetGridStructure(int Index, GridStructure Structure) {
	GridCellData[Index].GridStructure = Structure;
}

bool AGridManager::IsValidStationSpawn(int Coord) {
	return false;
}

