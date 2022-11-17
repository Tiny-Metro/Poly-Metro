// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGenerator/GridManager.h"
#include <Kismet/KismetMathLibrary.h>

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
	FIntPoint tmp;

	return FGridCellData();
}

void AGridManager::SetGridStructure(int X, int Y, GridStructure Structure) {
	GridCellData[(GridSize.X * Y) + X].GridStructure = Structure;
	GridStructures.Add(TPair<FIntPoint, GridStructure>(FIntPoint(X, Y), Structure));
	if (Structure == GridStructure::Station) {
		StationLocation.Add(FIntPoint(X, Y));
	}
}

bool AGridManager::IsValidStationSpawn(int Coord) {
	if (GridCellData[Coord].GridStructure == GridStructure::Empty) {
		return true;
	}

	return false;
}

TPair<FVector2D, double> AGridManager::FindCircleWith2Points(FVector2D P1, FVector2D P2, int Index, int Index2) {
	TPair<FVector2D, double> CurrentCircle;
	FVector2D TempCenter = FindCenter(P1, P2);
	CurrentCircle = TPair<FVector2D, double>(TempCenter, GetDistance(TempCenter, P1));

	for (int i = 0; i < Index2; i++) {
		if (GetDistance(StationLocation[i], CurrentCircle.Key) > CurrentCircle.Value) {
			FVector2D Temp = FindCenter(P1, P2, StationLocation[i]);
			CurrentCircle = TPair<FVector2D, double>(Temp, GetDistance(Temp, StationLocation[i]));
		}
	}

	return CurrentCircle;
}

TPair<FVector2D, double> AGridManager::FindCircleWithPoints(FVector2D P1, int Index) {
	TPair<FVector2D, double> CurrentCircle;
	FVector2D TempCenter = FindCenter(P1, StationLocation[0]);
	CurrentCircle = TPair<FVector2D, double>(TempCenter, GetDistance(TempCenter, P1));

	for (int i = 1; i < Index; i++) {
		if (GetDistance(StationLocation[i], CurrentCircle.Key) > CurrentCircle.Value) {
			CurrentCircle = FindCircleWith2Points(P1, StationLocation[i], Index, i);
		}
	}

	return CurrentCircle;
}

TPair<FVector2D, double> AGridManager::FindCircle() {
	TPair<FVector2D, double> CurrentCircle;
	FVector2D TempCenter = FindCenter(StationLocation[0], StationLocation[1]);
	CurrentCircle = TPair<FVector2D, double>(TempCenter, GetDistance(TempCenter, StationLocation[0]));

	for (int i = 2; i < StationLocation.Num(); i++) {
		if (GetDistance(StationLocation[i], CurrentCircle.Key) > CurrentCircle.Value) {
			CurrentCircle = FindCircleWithPoints(StationLocation[i], i);
		}
	}

	return CurrentCircle;
}

FVector2D AGridManager::FindCenter(FVector2D A, FVector2D B) {
	return (A+B)/2;
}

FVector2D AGridManager::FindCenter(FVector2D A, FVector2D B, FVector2D C) {
	return (A+B+C)/3;
}

double AGridManager::GetDistance(FVector2D A, FVector2D B) {
	return UKismetMathLibrary::Distance2D(A, B);
}

