// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/Lane.h"
#include "Train/TrainTemplate.h"
#include "Station/Station.h"
#include "GridGenerator/GridCellData.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ALane::ALane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	
	StationManagerRef = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));


	LaneMaterial.AddUnique(
		ConstructorHelpers::FObjectFinder<UMaterial>(*LaneDefaultMaterialPath).Object
	);
}

// Called when the game starts or when spawned
void ALane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALane::SetLaneId(int _LaneId)
{
	LaneId = _LaneId;
}

void ALane::InitLaneMaterial(TArray<UMaterial*> Materials) {
	LaneMaterial.Append(Materials);
}

bool ALane::GetIsCircularLine() const
{
	return IsCircularLine;
}

void ALane::SetIsCircularLine(bool _Circular)
{
	IsCircularLine = _Circular;
}

bool ALane::GetAlreadyDeleted() const
{
	return AlreadyDeleted;
}

void ALane::SetAlreadyDeleted(bool _Delete)
{
	AlreadyDeleted = _Delete;
}

int32 ALane::GetLaneId() const
{
	return LaneId;
}

void ALane::CheckStationPoint() {

	tmpLaneArray0.Empty();
	tmpLaneArray1.Empty();
	LaneArray.Empty();

	for (int i = 0; i < StationPoint.Num(); i++) {

		FLanePoint tmp;
		tmp.Coordination = StationPoint[i]->GetCurrentGridCellData().WorldCoordination;
		tmp.IsBendingPoint = true;
		tmp.IsStation = true;

		tmpLaneArray0.Add(tmp);
	}
	/*
	for (int i = 0; i < tmpLaneArray0.Num(); i++) {

		UE_LOG(LogTemp, Warning, TEXT("tmpLaneArray0[%d] : X = %d , Y = %d"), i, tmpLaneArray0[i].Coordination.X, tmpLaneArray0[i].Coordination.Y);

	}

	UE_LOG(LogTemp, Error, TEXT("CheckStationPoint End!"));
	*/
}

void ALane::SetBendingPoint() {
	
	for (int i = 0; i < tmpLaneArray0.Num(); i++) {

		tmpLaneArray1.Add(tmpLaneArray0[i]);

		int j = i + 1;

		if ( j < tmpLaneArray0.Num()) {
			FIntPoint tmpTarget;

			if (FindBendingPoint(tmpTarget, tmpLaneArray0[i].Coordination, tmpLaneArray0[j].Coordination)) {

				FLanePoint tmp;
				tmp.Coordination = tmpTarget;
				tmp.IsBendingPoint = true;

				tmpLaneArray1.Add(tmp);

			}
		}
	}
	/*
	for (int i = 0; i < tmpLaneArray1.Num(); i++) {
		UE_LOG(LogTemp, Warning, TEXT("tmpLaneArray1[%d] : X = %d , Y = %d"), i, tmpLaneArray1[i].Coordination.X, tmpLaneArray1[i].Coordination.Y);

	}

	UE_LOG(LogTemp, Error, TEXT("SetBendingPoint End!"));
	*/
}

bool ALane::FindBendingPoint( FIntPoint& TargetPoint,FIntPoint PointStart, FIntPoint PointEnd) {

	if (PointStart.X == PointEnd.X || PointStart.Y == PointEnd.Y) {
		TargetPoint.X = 0;
		TargetPoint.Y = 0;
		return false;
	}
	else {
		FIntPoint PointDistance;
		PointDistance = PointEnd - PointStart;

		int32 AbsDistanceX = abs(PointDistance.X);
		int32 AbsDistanceY = abs(PointDistance.Y);

		if (AbsDistanceX == AbsDistanceY) {
			TargetPoint.X = 0;
			TargetPoint.Y = 0;
			return false;
		}
		else if (AbsDistanceX > AbsDistanceY) {

			int32 tmp = PointDistance.X / AbsDistanceX;

			tmp = tmp * AbsDistanceY;

			TargetPoint.X = PointStart.X + tmp;
				
			TargetPoint.Y = PointStart.Y + PointDistance.Y;

			return true;
		}else {

				int32 tmp = PointDistance.Y / AbsDistanceY;

				tmp = tmp * AbsDistanceX;

				TargetPoint.X = PointStart.X + PointDistance.X;
				TargetPoint.Y = PointStart.Y + tmp;
				return true;
			
 		}
	}

	//UE_LOG(LogTemp, Error, TEXT("FindBendingPoint End!"));
}

void ALane::FillLanePoint() {
	for (int i = 0; i < tmpLaneArray1.Num(); i++) {
		
		LaneArray.Add(tmpLaneArray1[i]);

		TmpLanePoint0 = tmpLaneArray1[i];

		if ( i < (tmpLaneArray1.Num() - 1)) {
			TmpLanePoint1 = tmpLaneArray1[i + 1];

			FIntPoint DistancePoint =  TmpLanePoint1.Coordination - tmpLaneArray1[i].Coordination;

			int32 AbsDistancePointX = abs(DistancePoint.X);
			int32 AbsDistancePointY = abs(DistancePoint.Y);

			if (AbsDistancePointX > 1 || AbsDistancePointY > 1) {

				int32 TmpX, TmpY;

				if (AbsDistancePointX >= AbsDistancePointY) {

					for (int j = 1; j < AbsDistancePointX; j++) {

						if (DistancePoint.X == 0) {
							TmpX = TmpLanePoint0.Coordination.X;
						}
						else if (DistancePoint.X > 0) {
							TmpX = TmpLanePoint0.Coordination.X + j;
						}
						else {
							TmpX = TmpLanePoint0.Coordination.X + (j * (-1));
						}

						if (DistancePoint.Y == 0) {
							TmpY = TmpLanePoint0.Coordination.Y;
						}
						else if (DistancePoint.Y > 0) {
							TmpY = TmpLanePoint0.Coordination.Y + j;
						}
						else {
							TmpY = TmpLanePoint0.Coordination.Y + (j * (-1));
						}

						FLanePoint AddTmp;

						AddTmp.Coordination.X = TmpX;
						AddTmp.Coordination.Y = TmpY;


						LaneArray.Add(AddTmp);

					}
				}
				else {
					for (int j = 1; j < AbsDistancePointY; j++) {

						if (DistancePoint.X == 0) {
							TmpX = TmpLanePoint0.Coordination.X;
						}
						else if (DistancePoint.X > 0) {
							TmpX = TmpLanePoint0.Coordination.X + j;
						}
						else {
							TmpX = TmpLanePoint0.Coordination.X + (j * (-1));
						}

						if (DistancePoint.Y == 0) {
							TmpY = TmpLanePoint0.Coordination.Y;
						}
						else if (DistancePoint.Y > 0) {
							TmpY = TmpLanePoint0.Coordination.Y + j;
						}
						else {
							TmpY = TmpLanePoint0.Coordination.Y + (j * (-1));
						}

						FLanePoint AddTmp;

						AddTmp.Coordination.X = TmpX;
						AddTmp.Coordination.Y = TmpY;


						LaneArray.Add(AddTmp);	

					}
					
				}
			}
		}

	}

	/*
	for (int i = 0; i < LaneArray.Num(); i++) {
		UE_LOG(LogTemp, Error, TEXT("LaneArray[%d] : X = %d , Y = %d"), i, LaneArray[i].Coordination.X , LaneArray[i].Coordination.Y);

	}
	
	UE_LOG(LogTemp, Error, TEXT("FillLanePoint End!"));
	*/
}

void ALane::LaneCreating_Implementation() {}

void ALane::InitializeNewLane_Implementation() {}

void ALane::ExtendLane_Implementation() {}

FIntPoint ALane::GetNextLocation(class ATrainTemplate* Train, FIntPoint CurLocation, TrainDirection Direction)
{
	int32 Index = -1;

	for (int32 i = 0; i < LaneArray.Num(); i++) {

		if (LaneArray[i].Coordination == CurLocation) {

			Index = i;

			break;
		}
	}

	if (Index == -1) {

		UE_LOG(LogTemp, Warning, TEXT("Couldn't find Index in GetNextLocation"))
		return FIntPoint();
	}

	UE_LOG(LogTemp, Warning, TEXT("Train Direction : %d"), Direction);
	UE_LOG(LogTemp, Warning, TEXT("Index Num : %d"), Index);

	if (Direction == TrainDirection::Up) { // index get smaller

		if (Index != 0) {

			for (int32 i = Index -1; i >= 0; i--) {
				if (LaneArray[i].IsStation) {
					UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / Train Direction is Up : %d"), i);
					return LaneArray[i].Coordination;
				}
			}

			//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num : Index-1 / %d"), Index - 1);
			return LaneArray[Index - 1].Coordination;
		}
		else {
			Train->SetTrainDirection(TrainDirection::Down);
			for (int32 i = Index + 1; i < LaneArray.Num(); i++) {
				if (LaneArray[i].IsStation) {
					//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / This Index is 0 : %d"), i);
					return LaneArray[i].Coordination;
				}
			}
			return LaneArray[1].Coordination;
		}

	}
	else {

		if (Index != (LaneArray.Num() - 1)) {
			for (int32 i = Index + 1; i < LaneArray.Num(); i++) {
				if (LaneArray[i].IsStation) {
					UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / Train Direction is Down : %d"), i);
					return LaneArray[i].Coordination;
					
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / Index+1 : %d"), Index +1);
			return LaneArray[Index + 1].Coordination;
		}
		else {
			Train->SetTrainDirection(TrainDirection::Up);
			for (int32 i = Index - 1; i >= 0; i--) {
				if (LaneArray[i].IsStation) {
					//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / This Index is Last Index : %d"), i);
					return LaneArray[i].Coordination;
				}
			}
			//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num : Index-1 / %d"), Index - 1);
			return LaneArray[Index - 1].Coordination;
		}
	}

}

TrainDirection ALane::SetDirectionInit(AStation* Station, FIntPoint CurLocation) const
{
	int32 StationIndex = -1;
	int32 CurLocationIndex = -1;

	for (int i = 0; i < LaneArray.Num(); i++) {
		if (Station->GetCurrentGridCellData().WorldCoordination == LaneArray[i].Coordination) {
			StationIndex = i;
		}

		if (CurLocation == LaneArray[i].Coordination) {
			CurLocationIndex = i;
		}
	}

	if (StationIndex == -1 || CurLocationIndex == -1) {

		UE_LOG(LogTemp, Warning, TEXT("Couldn't find TrainDirection in SetDirectionInit"));
		return TrainDirection();
	}

	if (StationIndex < CurLocationIndex) {
		return TrainDirection::Up;
	}
	else {
		return TrainDirection::Down;
	}

}

void ALane::SpawnTrain()
{
}

AStation* ALane::GetNextStation(AStation* CurrStation, TrainDirection Direction)
{
	int Index = -1;
	for (int i = 0; i < StationPoint.Num(); i++) {
		
		if (CurrStation == StationPoint[i]) {
			Index = i;
			break;
		}
	}

	if (Index == -1) {

		UE_LOG(LogTemp, Warning, TEXT("Couldn't find Index in GetNextStation"))
		return nullptr;
	}



	if (!GetIsCircularLine()) {

		if (Index == StationPoint.Num() - 1) {
			return StationPoint[Index-1];
		}
		else if (Index == 0) {
			return StationPoint[1];
		}
		else {
			if (Direction == TrainDirection::Down) {
				return StationPoint[Index + 1];
			}
			else {
				return StationPoint[Index - 1];
			}
		}
	}
	else { // circular
		if (Direction == TrainDirection::Down) {
			if (Index == StationPoint.Num() - 1) {
				return StationPoint[1];
			}
			else {
				return StationPoint[Index + 1];
			}

		}
		else {
			if (Index == 0) {
				return StationPoint[StationPoint.Num() - 2];
			}
			else {
				return StationPoint[Index - 1];
			}
		}
	}
	
	/*
	if (Direction == TrainDirection::Down) {
		if (Index == StationPoint.Num() - 1) {
			return StationPoint[Index + 1];
		}
		else {
			return StationPoint[Index + 1];
		}
		
	} else {
		if (Index == 0) {
			return StationPoint[1];
		}
		else {
			return StationPoint[Index - 1];
		}
	}
	*/
}

void ALane::SetGridLaneStructure()
{
	for (int i = 0; i < LaneArray.Num(); i++) {

		FIntPoint Coordination = LaneArray[i].Coordination;

		FGridCellData GridCellData = GridManagerRef->GetGridCellDataByPoint(Coordination.X, Coordination.Y);


		switch (GridCellData.GridType) {
		case GridType::Ground:
			GridManagerRef->SetGridLane(Coordination.X, Coordination.Y, GridLaneStructure::Lane);
			break;
		case GridType::Water:
			GridManagerRef->SetGridLane(Coordination.X, Coordination.Y, GridLaneStructure::Bridge);
			break;
		case GridType::Hill:
			GridManagerRef->SetGridLane(Coordination.X, Coordination.Y, GridLaneStructure::Turnel);
			break;
		}
	}
	
}

void ALane::AddAdjListDistance(AStation* First, AStation* Second)
{
	FIntPoint Start = First->GetCurrentGridCellData().WorldCoordination;
	FIntPoint End = Second->GetCurrentGridCellData().WorldCoordination;


	int N = abs(Start.X - End.X);
	int M = abs(Start.Y - End.Y);

	float Distance;

	if (N < M) {
		Distance = (N * 14)/10.0 + M - N;
	}
	else {
		Distance = (M * 14) / 10.0 + N - M;
	}

	

	UE_LOG(LogTemp, Warning, TEXT("AddAdjListDistance /IntPoint Start : %d, %d"), Start.X, Start.Y);
	UE_LOG(LogTemp, Warning, TEXT("AddAdjListDistance /IntPoint End : %d, %d"), End.X, End.Y);

	/*
	if (First == nullptr) {
		First = StationManagerRef->GetStationByGridCellData(Start);

		UE_LOG(LogTemp, Warning, TEXT("IntPoint : %d / %d"), Start.X, Start.Y);
		//UE_LOG(LogTemp, Warning, TEXT("Station Id : %d"), First->GetStationId());
	}

	if (Second == nullptr) {
		Second = StationManagerRef->GetStationByGridCellData(End);

		UE_LOG(LogTemp, Warning, TEXT("Station Id : %d"), Second->GetStationId());
	}
	*/

	StationManagerRef->AddAdjListItem(First, Second, Distance);

}

FIntPoint ALane::GetWorldCoordinationByStationPointIndex(int32 Index)
{
	FIntPoint Result = StationPoint[Index]->GetCurrentGridCellData().WorldCoordination;
	return StationPoint[Index]->GetCurrentGridCellData().WorldCoordination;
}
