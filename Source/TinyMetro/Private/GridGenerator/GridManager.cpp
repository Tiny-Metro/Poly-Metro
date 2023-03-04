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

FIntPoint AGridManager::GetGridSize() const
{
	return GridSize;
}

TArray<FGridCellData> AGridManager::GetGridCellData() const {
	// // O: insert return statement here
	return GridCellData;
}

int32 AGridManager::GetStationSpawnPrevent() const
{
	return StationSpawnPrevent;
}

FGridCellData AGridManager::GetGridCellDataByCoord(FVector Coord, bool& Succeess) const {
	Succeess = false;
	FIntPoint ApproxCoord(
		CoordApproximation(Coord.X, GridSize.X % 2 == 0),
		CoordApproximation(Coord.Y, GridSize.Y % 2 == 0)
	);
	for (auto i : GridCellData) {
		FIntPoint tmp(i.WorldLocation.X, i.WorldLocation.Y);
		if (tmp == ApproxCoord) {
			Succeess = true;
			return i;
		}
	}

	return GetGridCellDataByPoint(GridSize.X / 2, GridSize.Y / 2);
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
	UE_LOG(LogTemp, Log, TEXT("GridManager::GetGridCellDataByIndex"));
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

FGridCellData AGridManager::GetGridCellDataRandom() {
	TPair<FVector2D, double> Temp = FindCircle();
	FIntPoint Point(Temp.Key.X, Temp.Key.Y);
	int32 Radius = Temp.Value;
	FIntPoint RandomPoint;
	bool Flag = true;

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.0f,
		FColor::Green,
		FString::Printf(TEXT("FindCircle : %d %d %d"), Point.X, Point.Y, Radius)
	);

	do {
		RandomPoint = FIntPoint(
			FMath::RandRange(-Radius - StationSpawnExtend, Radius + StationSpawnExtend),
			FMath::RandRange(-Radius - StationSpawnExtend, Radius + StationSpawnExtend));
		if (RandomPoint.Size() > Radius + StationSpawnExtend) continue;
		Flag = !IsValidStationSpawn(RandomPoint.X + Point.X, RandomPoint.Y + Point.Y);
	} while (Flag);

	//return FGridCellData();
	return GridCellData[(GridSize.X * (RandomPoint.Y + Point.Y)) + (RandomPoint.X + Point.X)];
}

void AGridManager::SetGridStructure(int X, int Y, GridStructure Structure) {
	GridCellData[(GridSize.X * Y) + X].GridStructure = Structure;
	//GridStructures.Add(TPair<FIntPoint, GridStructure>(FIntPoint(X, Y), Structure));
	if (Structure == GridStructure::Station) {
		StationLocation.Add(FIntPoint(X, Y));
	}
}

void AGridManager::SetGridStation(int X, int Y, GridStationStructure Structure) {
	GridCellData[(GridSize.X * Y) + X].StationInfo = Structure;
	if (Structure == GridStationStructure::Station) {
		StationLocation.Add(FIntPoint(X, Y));
	}
}

void AGridManager::SetGridLane(int X, int Y, GridLaneStructure Structure) {
	GridCellData[(GridSize.X * Y) + X].LaneInfo = Structure;
}

bool AGridManager::IsValidStationSpawn(int Coord) {
	// Out of range
	if (Coord >= GridCellData.Num() || Coord < 0) return false;
	// Grid is empty
	if (GridCellData[Coord].StationInfo != GridStationStructure::Empty) return false;
	// Grid is ground
	if (GridCellData[Coord].GridType != GridType::Ground) return false;
	// Check other station
	for (int i = -StationSpawnPrevent; i <= StationSpawnPrevent; i++) { // Y
		for (int j = -StationSpawnPrevent; j <= StationSpawnPrevent; j++) { // X
			if (FMath::Sqrt(i * i + j * j) > StationSpawnPrevent) continue;
			if ((Coord + (i * GridSize.X) + j) >= GridCellData.Num() || (Coord + (i * GridSize.X) + j) < 0) continue;
			if (GridCellData[(Coord + (i * GridSize.X) + j)].StationInfo == GridStationStructure::Station) return false;
			//if (((GridSize.X * (Y + i)) + (X + j)) >= GridCellData.Num()) continue;
			//if (GridCellData[(GridSize.X * (Y + i)) + (X + j)].GridStructure == GridStructure::Station) return false;
		}
	}

	return true;
}

bool AGridManager::IsValidStationSpawn(int X, int Y) {
	// Out of range
	if (X >= GridSize.X || Y >= GridSize.Y || X < 0 || Y < 0) return false;
	// Grid is empty
	if (GridCellData[(GridSize.X * Y) + X].StationInfo != GridStationStructure::Empty) return false;
	// Grid is ground
	if (GridCellData[(GridSize.X * Y) + X].GridType != GridType::Ground) return false;
	// Check other station
	for (int i = -StationSpawnPrevent; i <= StationSpawnPrevent; i++) { // Y
		for (int j = -StationSpawnPrevent; j <= StationSpawnPrevent; j++) { // X
			if (FMath::Sqrt(i * i + j * j) > StationSpawnPrevent) continue;
			if (((GridSize.X * (Y + i)) + (X + j)) >= GridCellData.Num() || ((GridSize.X * (Y + i)) + (X + j)) < 0) continue;
			if (GridCellData[(GridSize.X * (Y + i)) + (X + j)].StationInfo == GridStationStructure::Station) return false;
		}
	}

	return true;
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

int32 AGridManager::CoordApproximation(double Coord, bool Flag) const {
	if (!Flag) Coord += ((GridCellSize / 2) * (Coord >= 0 ? 1 : -1));
	int tmp = Coord / GridCellSize;
	int result = tmp * GridCellSize;
	if (Flag) result += ((GridCellSize / 2) * (Coord >= 0 ? 1 : -1));

	return result;
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

