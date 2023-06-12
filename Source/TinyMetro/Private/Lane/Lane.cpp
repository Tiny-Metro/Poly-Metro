// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/Lane.h"
#include "Train/TrainTemplate.h"
#include "Train/TrainManager.h"
#include "Train/Train.h"
#include "Station/Station.h"
#include "GridGenerator/GridCellData.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Controller.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ALane::ALane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	
	StationManagerRef = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));

	TrainManagerRef = Cast<ATrainManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrainManager::StaticClass()));

	LaneMaterial.AddUnique(
		ConstructorHelpers::FObjectFinder<UMaterial>(*LaneDefaultMaterialPath).Object
	);
}

// Called when the game starts or when spawned
void ALane::BeginPlay()
{
	Super::BeginPlay();

	TinyMetroPlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	
}

// Called every frame
void ALane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//Delay Removing
	if (DoesLaneToBeRemoved)
	{
		FinishClearingLane();
	}

	if (DoesStationsToBeRemovedAtStart)
	{
		FinishRemovingLaneAtStart();
	}

	if (DoesStationsToBeRemovedAtEnd)
	{ 
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, TEXT("Stations Have To Be Removed At End"));
		FinishRemovingLaneAtEnd();
	}

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

FLanePoint ALane::GetNearestLanePoint(FVector Location) {
	double Distance = std::numeric_limits<double>::max();
	FLanePoint tmp;
	for (auto& i : LaneArray) {
		auto Grid = GridManagerRef->GetGridCellDataByPoint(i.Coordination.X, i.Coordination.Y);
		double tmpDist = FVector::Distance(this->GetActorLocation(), Grid.WorldLocation);
		if (Distance > tmpDist) {
			Distance = tmpDist;
			tmp = i;
		}
	}

	return tmp;
}

LaneDirection ALane::GetLaneShape(FVector Location) {
	FIntPoint min = LaneArray[0].Coordination;
	FIntPoint secondMin = LaneArray[0].Coordination;
	double distanceMin = std::numeric_limits<double>::max();
	double secondDistanceMin = std::numeric_limits<double>::max();

	for (auto& i : LaneArray) {
		auto Grid = GridManagerRef->GetGridCellDataByPoint(i.Coordination.X, i.Coordination.Y);
		double tmpDist = FVector::Distance(Location, Grid.WorldLocation);
		if (tmpDist < distanceMin) {
			secondMin = min;
			secondDistanceMin = distanceMin;
			min = i.Coordination;
			distanceMin = tmpDist;
		} else if (tmpDist < secondDistanceMin && tmpDist != distanceMin) {
			secondMin = i.Coordination;
			secondDistanceMin = tmpDist;
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		FString::Printf(TEXT("Lane::GetLaneShape %d, %d / %d, %d"), min.X, min.Y, secondMin.X, secondMin.Y)
	);

	auto shape = min - secondMin;
	LaneDirection result;
	if (shape.X == 0) { // Vertical
		result = LaneDirection::Vertical;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Lane::GetLaneShape Vertical"))
		);
	} else if (shape.Y == 0) { // Horizontal
		result = LaneDirection::Horizontal;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Lane::GetLaneShape Horizontal"))
		);
	} else if (shape.X * shape.Y == -1) { // Y = -X
		result = LaneDirection::DiagonalR;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Lane::GetLaneShape DiagonalR (Y = X)"))
		);
	} else if (shape.X * shape.Y == 1) { // Y = X
		result = LaneDirection::DiagonalL;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Lane::GetLaneShape DiagonalL (Y = -X)"))
		);
	}

	return result;
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

void ALane::FinishRemovingLaneAtStart_Implementation() {}

void ALane::FinishRemovingLaneAtEnd_Implementation() {}

void ALane::FinishClearingLane_Implementation() {}



bool ALane::CheckTrainsByDestination(const TArray <class AStation*>& Stations)
{
	bool res = false;

	for (AStation* Station : Stations)
	{
		int32 tmp = TrainManagerRef->GetStationCountByDestination(Station->GetStationInfo(), this);

		tmp += TrainManagerRef->GetStationCountByOrigin(Station->GetStationInfo(), this);

		if (tmp != 0)
		{
			res = true;
			break;
		}

		
	}

	return res;
}

void ALane::NotifyTrainsOfRemoving(const TArray<class AStation*>& Stations)
{
	for (AStation* Station : Stations)
	{
		TrainManagerRef->TrainDeferredDespawn(Station->GetStationInfo(), this);
		//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("DEBUG")));
	}
}

void ALane::ModifyStationInfoWhenRemoving(const TArray <class AStation*>& Stations)
{
	for (AStation* Station : Stations)
	{
		
		Station->SetActivate(false);
		Station->RemoveLane(LaneId);
	}
}

TArray<class AStation *> ALane::CollectEveryStations()
{
	TArray<AStation* > EveryStations;

	for(auto Station : StationPointBeforeRemovedStart)
	{
		EveryStations.Add(Station);
	}

	for(auto Station : StationPointBeforeRemovedEnd)
	{
		EveryStations.AddUnique(Station);
	}

	return EveryStations;
}

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

	if (!GetIsCircularLine()) {

		if (Direction == TrainDirection::Up) { // index get smaller

			if (Index != 0) {

				for (int32 i = Index - 1; i >= 0; i--) {
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
	else { // circular
		if (Direction == TrainDirection::Up) { // index get smaller

			if (Index != 0) {

				for (int32 i = Index - 1; i >= 0; i--) {
					if (LaneArray[i].IsStation) {
						UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / Train Direction is Up : %d"), i);
						return LaneArray[i].Coordination;
					}
				}

				//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num : Index-1 / %d"), Index - 1);
				return LaneArray[Index - 1].Coordination;
			}
			else {
				for (int32 i = LaneArray.Num() -2 ; i >=0; i--) {
					if (LaneArray[i].IsStation) {
						//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / This Index is 0 : %d"), i);
						return LaneArray[i].Coordination;
					}
				}
				return LaneArray[LaneArray.Num()-2].Coordination;
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
				for (int32 i = 1; i < LaneArray.Num(); i++) {
					if (LaneArray[i].IsStation) {
						//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num / This Index is Last Index : %d"), i);
						return LaneArray[i].Coordination;
					}
				}
				//UE_LOG(LogTemp, Warning, TEXT("Next Location Index Num : Index-1 / %d"), Index - 1);
				return LaneArray[1].Coordination;
			}
		}
	}


	

}

TrainDirection ALane::SetDirectionInit(AStation* Station, FIntPoint CurLocation) const
{
	int32 StationIndex = -1;
	int32 CurLocationIndex = -1;

	for (int i = 0; i < LaneArray.Num(); i++) {
		if (Station->GetCurrentGridCellData().WorldCoordination == LaneArray[i].Coordination) {
			UE_LOG(LogTemp, Warning, TEXT("Station Index is : %d"), i);
			StationIndex = i;
		}

		if (CurLocation == LaneArray[i].Coordination) {
			CurLocationIndex = i;
		}
	}

	if (GetIsCircularLine()) {
		if (StationIndex == LaneArray.Num() -1) {
			if (((LaneArray.Num() - 1) - CurLocationIndex) > CurLocationIndex) {
				StationIndex = 0;
			}
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
	if (TinyMetroPlayerState->UseTrain()) {

		UWorld* world = GetWorld();

		

		if (world != nullptr)
		{
			world->GetTimerManager().SetTimer(
				SpawnTrainCheckTimer,
				FTimerDelegate::CreateLambda([&]() {

					UE_LOG(LogTemp, Warning, TEXT("SpawnTrain Timer"));
					
					AStation* Destination = StationPoint[1];

					UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Train/BP_Train.BP_Train'")));

					UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
					UClass* SpawnClass = SpawnActor->StaticClass();

					FActorSpawnParameters SpawnParams;
					FTransform SpawnTransform;

					FVector SpawnLocation = StationPoint[0]->GetCurrentGridCellData().WorldLocation;
					SpawnLocation = { SpawnLocation.X, SpawnLocation.Y, 20 };

					SpawnTransform.SetLocation(SpawnLocation);

					ATrain* Train = Cast<ATrain>(GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnTransform));


					Train->ServiceStart(Train->GetActorLocation(), this, Destination);
					
					}),
				1.0f,
				false,
				0.5f
			);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("ERROR! SpawnTrain Timer"));
		}

		
	}
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

//REFACTORING SET LANE ARRAY
void ALane::RSetLaneArray(const TArray<class AStation*>& NewStationPoint) {

	StationPoint = NewStationPoint;
	ClearLanePoint();

	//int32 NumStations = NewStationArray.Num();
	int32 NumStations = NewStationPoint.Num();

	for (int32 i = 0; i < NumStations; i++) {
		
		FIntPoint CurrentStation = NewStationPoint[i]->GetCurrentGridCellData().WorldCoordination;

		FLanePoint CurrentLanePoint;
		CurrentLanePoint.Coordination = CurrentStation;
		CurrentLanePoint.IsStation = true;
		CurrentLanePoint.IsBendingPoint = true;
		CurrentLanePoint.IsThrough = false;

		RLaneArray.Add(CurrentLanePoint);

		if (i < NumStations - 1) {
			FIntPoint NextStation = NewStationPoint[i+1]->GetCurrentGridCellData().WorldCoordination;
			FIntPoint Diff = NextStation - CurrentStation;

			FIntPoint BendingCoord;
			bool HasBendingPoint = hasBendingPoint(CurrentStation, NextStation);
			
			if (HasBendingPoint) {
				BendingCoord = findBendingPoint(CurrentStation, NextStation);

				TArray<FIntPoint> PathToBending = GeneratePath(CurrentStation, BendingCoord);
				TArray<FIntPoint> PathFromBending = GeneratePath(BendingCoord, NextStation);

				for (const FIntPoint& Point : PathToBending) {
					FLanePoint PathPoint;
					PathPoint.Coordination = Point;
					PathPoint.IsStation = false;
					PathPoint.IsBendingPoint = false;
					if(GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
						PathPoint.IsThrough = true;
					}
					else PathPoint.IsThrough = false;

					RLaneArray.Add(PathPoint);
				}

				FLanePoint BendingPoint;
				BendingPoint.Coordination = BendingCoord;
				BendingPoint.IsStation = false;
				BendingPoint.IsBendingPoint = true;
				if (GridManagerRef->GetGridCellDataByPoint(BendingCoord.X, BendingCoord.Y).StationInfo == GridStationStructure::Station) {
					BendingPoint.IsThrough = true;
				}
				else BendingPoint.IsThrough = false;

				RLaneArray.Add(BendingPoint);
				
				for (const FIntPoint& Point : PathFromBending)
				{
					FLanePoint PathPoint;
					PathPoint.Coordination = Point;
					PathPoint.IsStation = false;
					PathPoint.IsBendingPoint = false;
					if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
						PathPoint.IsThrough = true;
					}
					else PathPoint.IsThrough = false;

					RLaneArray.Add(PathPoint);
				}

			}
			else {
				TArray<FIntPoint> PathToNext = GeneratePath(CurrentStation, NextStation);

				for (const FIntPoint& Point : PathToNext)
				{
					FLanePoint PathPoint;
					PathPoint.Coordination = Point;
					PathPoint.IsStation = false;
					PathPoint.IsBendingPoint = false;
					if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
						PathPoint.IsThrough = true;
					}
					else PathPoint.IsThrough = false;

					RLaneArray.Add(PathPoint);
				}
			}


		}


	}
}

bool ALane::hasBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation) {
	FIntPoint Diff = NextStation - CurrentStation;

	if (Diff.X == 0) return false;
	if (Diff.Y == 0) return false;
	if (FMath::Abs(Diff.X) == FMath::Abs(Diff.Y)) return false;
	return true;
}

FIntPoint ALane::findBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation) {
	FIntPoint BendingPoint;
	
	FIntPoint Diff = NextStation - CurrentStation;

	if (FMath::Abs(Diff.X) > FMath::Abs(Diff.Y)) {
		BendingPoint.X = CurrentStation.X + FMath::Sign(Diff.X) * FMath::Abs(Diff.Y);
		BendingPoint.Y = NextStation.Y;
	}
	else {
		BendingPoint.X = NextStation.X;
		BendingPoint.Y = CurrentStation.Y + FMath::Sign(Diff.Y) * FMath::Abs(Diff.X);
	}

	return BendingPoint;
}

TArray<FIntPoint> ALane::GeneratePath(const FIntPoint& Start, const FIntPoint& End){
	TArray<FIntPoint> Path;

	FIntPoint Diff = End - Start;
	FIntPoint Step(FMath::Sign(Diff.X), FMath::Sign(Diff.Y));
	FIntPoint Current = Start + Step;

	while (Current != End)
	{
		Path.Add(Current);
		Current += Step;
	}
	return Path;
}

//Refactoring SetLaneLocation
void ALane::RSetLaneLocation() {

	if (!GridManagerRef){
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is not valid."));
		return;
	}

	RLaneLocation.Empty();
	for (const FLanePoint& Point : RLaneArray){
		FVector VectorLocation = PointToLocation(Point.Coordination);
		RLaneLocation.Add(VectorLocation);		
	}

}

// REFACTORING SetLaneVector
FVector ALane::PointToLocation(const FIntPoint& Point) {
	return GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).WorldLocation;
}

// Refactoring clearSplineMesh
void ALane::ClearLanePoint() {
	// Clear the existing lane array if any
	for (int32 i = 0; i < RLaneArray.Num(); i++) {
		ClearSplineMeshAt(i);
	}
	RLaneArray.Empty();
}

void ALane::SetLaneSpline(USplineComponent* Spline) {
	Spline->SetSplinePoints(RLaneLocation, ESplineCoordinateSpace::World,true);

	for (int32 i = 0; i < RLaneLocation.Num(); i++) {
		Spline->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
}

void ALane::HandleScaling(bool IsScaling, float SectionLength) {
	if (IsScaling) { RSectionLength = GetActorScale3D().X * SectionLength; }
	else RSectionLength = SectionLength;
	return;
}

void ALane::HandleFullLength(bool IsFullLength, USplineComponent* Spline) {
	if (IsFullLength) {
		EndLoop = FMath::TruncToInt(FMath::TruncToFloat(Spline->GetSplineLength() / RSectionLength))-1;
	}
	else {
		EndLoop = FMath::TruncToInt(FMath::TruncToFloat(Spline->GetSplineLength() / RSectionLength));
	}
}

//Refactoring AddSplineMeshComponent

void ALane::SetMeshMaterial() {
	MeshMaterial = LaneMaterial[LaneId];
}

void ALane::R2SplineMeshComponent(USplineComponent* Spline) {

	//Check the input parameter is valid
	if (!Spline)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid input parameters for R2SplineMeshComponent."));
		return;
	}

	// init number of Spline points
	int32 NumSplinePoints = Spline->GetNumberOfSplinePoints();

	//Set Start & End
	FVector StartPos;
	FVector StartTangent;
	FVector EndPos;
	FVector EndTangent;

	float Length;
	float ClampedLength;


	for (int32 i = 0; i < NumSplinePoints; i++) {
		//Set First Point of Spline
		if(i==0){
			/* --- Front Mesh Only --- */

			// Set Start/End Pos/Tangent
			StartPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			EndPos = ((StartPos + Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local))/2.0f);

			StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Length = StartTangent.Size();
			ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
			StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
			EndTangent = StartTangent;

			//Add&Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}

		//Set Last Point of Spline
		if(i==NumSplinePoints-1){
			/* --- Back Mesh Only --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			StartTangent = EndTangent;
			EndPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);

			//Add&Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}

		//Set Points between
		if (0 < i && i < NumSplinePoints - 1) {
			/* --- Back Mesh --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			EndPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			StartTangent = EndTangent;

			//Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			if(RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
			else{SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);}
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);

			/* --- Middle Mesh (if there is any) --- */
			if (RLaneArray[i].IsBendingPoint) {
				// Set Start/End Pos/Tangent
				StartPos = EndPos;
				EndTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

				//Set Spline Mesh Component (mesh)
				SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
				else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
				SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
				SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			
				RLaneArray[i].MeshArray.Add(SplineMeshComponent);
			}

			/* --- Front Mesh --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			StartTangent = EndTangent;
			if (RLaneArray[i].IsBendingPoint) { 
				StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local); 
				Length = EndTangent.Size();
				ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
				StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
			}
			EndPos = ((StartPos + Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local)) / 2.0f);
			EndTangent = StartTangent;

			//Set Spline Mesh Component (mesh)
			SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
			else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
		
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}
	}

}

void ALane::SetSplineMeshComponent(USplineMeshComponent* SplineMeshComponent, UStaticMesh* SplineMesh) {
	SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMeshComponent->SetStaticMesh(SplineMesh);

	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	//Set Material
	SplineMeshComponent->SetMaterial(0, MeshMaterial);
	//Set Axis
	SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X, false);

	SplineMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	SplineMeshComponent->RegisterComponent();
}

void ALane::SetMesh(UStaticMesh* Mesh, UStaticMesh* ThroughMesh) {
	RSplineMesh = Mesh;
	RThroughMesh = ThroughMesh;
}

void ALane::ClearSplineMeshAt(int32 Index){
	for (USplineMeshComponent* SplineMeshComponent : RLaneArray[Index].MeshArray) {
		if (SplineMeshComponent) {
			SplineMeshComponent->DestroyComponent();
		}
	}
}

void ALane::RemoveLaneFromStart(int32 Index, USplineComponent* Spline) {

	int32 tmpIndex = 0;
	while (tmpIndex <= Index) {
		//StationPoint.RemoveAt(0);

		//Lane Point
		int count = 1;
		for (int32 i = 1; i < RLaneArray.Num(); ++i)
		{
			if (RLaneArray[i].IsStation) { break; }
			else {
				ClearSplineMeshAt(i);
				RLaneArray.RemoveAt(i);
				--i; // Decrement index to account for removed element
				count++;
			}
		}

		ClearSplineMeshAt(0);
		RLaneArray.RemoveAt(0);

		//Spline Point
		for (int i = 0; i < count; i++) {
			Spline->RemoveSplinePoint(0);
			RLaneLocation.RemoveAt(0);
		}

		tmpIndex++;
	}

	//Set StartPoint's Mesh Again
	ClearSplineMeshAt(0);

	FVector StartPos = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector EndPos = ((StartPos + Spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local)) / 2.0f);

	FVector StartTangent = Spline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector EndTangent;
	float Length = StartTangent.Size();
	float ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
	StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
	EndTangent = StartTangent;

	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
	SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);
	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

	RLaneArray[0].MeshArray.Add(SplineMeshComponent);
}

void ALane::RemoveLaneFromEnd(int32 Index, int32 ExStationNum, USplineComponent* Spline) {

//	int32 tmpIndex = StationPoint.Num() -1;
	int32 tmpIndex = ExStationNum - 1;

	while (tmpIndex >= Index) {
		//int32 lastIndexStation= StationPoint.Num() - 1;
		//StationPoint.RemoveAt(lastIndexStation);

		//Lane Point
		int count = 1;
		for (int32 i = RLaneArray.Num() - 2; i >= 0; --i)
		{
			if (RLaneArray[i].IsStation) { break; }
			else {
				ClearSplineMeshAt(i);
				RLaneArray.RemoveAt(i);
				count++;
			}
		}

		ClearSplineMeshAt(RLaneArray.Num() - 1);
		RLaneArray.RemoveAt(RLaneArray.Num() - 1);

		//Spline Point
		for (int i = 0; i < count; i++) {
			Spline->RemoveSplinePoint(Spline->GetNumberOfSplinePoints()-1);
			RLaneLocation.RemoveAt(RLaneLocation.Num()-1);
		}

		tmpIndex--;
	}

	//Set EndPoint's Mesh Again
	int32 lastIndex = RLaneArray.Num() - 1;
	ClearSplineMeshAt(lastIndex);

	FVector EndPos = Spline->GetLocationAtSplinePoint(lastIndex, ESplineCoordinateSpace::Local);
	FVector StartPos = ((EndPos+ Spline->GetLocationAtSplinePoint(lastIndex-1, ESplineCoordinateSpace::Local)) / 2.0f);

	FVector StartTangent = Spline->GetTangentAtSplinePoint(lastIndex-1, ESplineCoordinateSpace::Local);
	FVector EndTangent;
	float Length = StartTangent.Size();
	float ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
	StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
	EndTangent = StartTangent;

	//Add&Set Spline Mesh Component (mesh)
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
	SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);
	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

	RLaneArray[lastIndex].MeshArray.Add(SplineMeshComponent);
}

void ALane::ExtendStart(AStation* NewStation, USplineComponent* Spline) {

//Destroy used-to-be first mesh
	ClearSplineMeshAt(0);

	//StationPoint.InsertDefaulted(0, 1);
	//StationPoint[0] = NewStation;

//Add Lane Array
	TArray<FLanePoint> AddLaneArray;

	FIntPoint NewPoint = NewStation->GetCurrentGridCellData().WorldCoordination;

	FLanePoint CurrentLanePoint;
	CurrentLanePoint.Coordination = NewPoint;
	CurrentLanePoint.IsStation = true;
	CurrentLanePoint.IsBendingPoint = true;
	CurrentLanePoint.IsThrough = false;

	AddLaneArray.Add(CurrentLanePoint);

	FIntPoint NextStation = StationPoint[1]->GetCurrentGridCellData().WorldCoordination;
	FIntPoint Diff = NextStation - NewPoint;

	FIntPoint BendingCoord;
	bool HasBendingPoint = hasBendingPoint(NewPoint, NextStation);

	if (HasBendingPoint) {
		BendingCoord = findBendingPoint(NewPoint, NextStation);

		TArray<FIntPoint> PathToBending = GeneratePath(NewPoint, BendingCoord);
		TArray<FIntPoint> PathFromBending = GeneratePath(BendingCoord, NextStation);

		for (const FIntPoint& Point : PathToBending) {
			FLanePoint PathPoint;
			PathPoint.Coordination = Point;
			PathPoint.IsStation = false;
			PathPoint.IsBendingPoint = false;
			if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
				PathPoint.IsThrough = true;
			}
			else PathPoint.IsThrough = false;

			AddLaneArray.Add(PathPoint);
		}

		FLanePoint BendingPoint;
		BendingPoint.Coordination = BendingCoord;
		BendingPoint.IsStation = false;
		BendingPoint.IsBendingPoint = true;
		if (GridManagerRef->GetGridCellDataByPoint(BendingCoord.X, BendingCoord.Y).StationInfo == GridStationStructure::Station) {
			BendingPoint.IsThrough = true;
		}
		else BendingPoint.IsThrough = false;

		AddLaneArray.Add(BendingPoint);

		for (const FIntPoint& Point : PathFromBending)
		{
			FLanePoint PathPoint;
			PathPoint.Coordination = Point;
			PathPoint.IsStation = false;
			PathPoint.IsBendingPoint = false;
			if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
				PathPoint.IsThrough = true;
			}
			else PathPoint.IsThrough = false;

			AddLaneArray.Add(PathPoint);
		}

	}
	else {
		TArray<FIntPoint> PathToNext = GeneratePath(NewPoint, NextStation);

		for (const FIntPoint& Point : PathToNext)
		{
			FLanePoint PathPoint;
			PathPoint.Coordination = Point;
			PathPoint.IsStation = false;
			PathPoint.IsBendingPoint = false;
			if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
				PathPoint.IsThrough = true;
			}
			else PathPoint.IsThrough = false;

			AddLaneArray.Add(PathPoint);
		}
	}

	RLaneArray.Insert(AddLaneArray, 0);

//Add LaneLoc
	TArray<FVector> NewLaneLocation;

	if (!GridManagerRef) {
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is not valid."));
		return;
	}
	for (const FLanePoint& Point : AddLaneArray) {
		FVector VectorLocation = PointToLocation(Point.Coordination);
		NewLaneLocation.Add(VectorLocation);
	}
	RLaneLocation.Insert(NewLaneLocation, 0);

//Set Spline Again
//	Spline->SetSplinePoints(RLaneLocation, ESplineCoordinateSpace::World, true);
	SetLaneSpline(Spline);
//Add Spline Mesh

	FVector StartPos;
	FVector StartTangent;
	FVector EndPos;
	FVector EndTangent;

	float Length;
	float ClampedLength;

	int32 NewPointNum = NewLaneLocation.Num();

	for (int32 i = 0; i <= NewPointNum; i++) {
		//Set First Point of Spline
		if (i == 0) {
			/* --- Front Mesh Only --- */

			// Set Start/End Pos/Tangent
			StartPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			EndPos = ((StartPos + Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local)) / 2.0f);

			StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
			Length = StartTangent.Size();
			ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
			StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
			EndTangent = StartTangent;

			//Add&Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}
		//Set Points between
		if (0 < i) {
			/* --- Back Mesh --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			EndPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			StartTangent = EndTangent;

			//Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
			else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);

			/* --- Middle Mesh (if there is any) --- */
			if (RLaneArray[i].IsBendingPoint) {
				// Set Start/End Pos/Tangent
				StartPos = EndPos;
				EndTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

				//Set Spline Mesh Component (mesh)
				SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
				else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
				SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
				SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
				RLaneArray[i].MeshArray.Add(SplineMeshComponent);
			}

			/* --- Front Mesh --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			StartTangent = EndTangent;
			if (RLaneArray[i].IsBendingPoint) {
				StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
				Length = EndTangent.Size();
				ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
				StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
			}
			EndPos = ((StartPos + Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local)) / 2.0f);
			EndTangent = StartTangent;

			//Set Spline Mesh Component (mesh)
			SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
			else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}
	}
}

void ALane::ExtendEnd(AStation* NewStation, USplineComponent* Spline) {
	//Destroy used-to-be last mesh
	ClearSplineMeshAt(RLaneArray.Num() -1);

	//StationPoint.Add(NewStation);

	//Add Lane Array
	TArray<FLanePoint> AddLaneArray;

	FIntPoint NewPoint = NewStation->GetCurrentGridCellData().WorldCoordination;

	FLanePoint CurrentLanePoint;
	CurrentLanePoint.Coordination = RLaneArray[RLaneArray.Num()-1].Coordination;
	CurrentLanePoint.IsStation = true;
	CurrentLanePoint.IsBendingPoint = true;
	CurrentLanePoint.IsThrough = false;


	FIntPoint NextStation = NewPoint;
	FIntPoint CurrentStation = CurrentLanePoint.Coordination;
	FIntPoint Diff = NextStation - CurrentStation;

	FIntPoint BendingCoord;
	bool HasBendingPoint = hasBendingPoint(CurrentStation, NextStation);

	if (HasBendingPoint) {
		BendingCoord = findBendingPoint(CurrentStation, NextStation);

		TArray<FIntPoint> PathToBending = GeneratePath(CurrentStation, BendingCoord);
		TArray<FIntPoint> PathFromBending = GeneratePath(BendingCoord, NextStation);

		for (const FIntPoint& Point : PathToBending) {
			FLanePoint PathPoint;
			PathPoint.Coordination = Point;
			PathPoint.IsStation = false;
			PathPoint.IsBendingPoint = false;
			if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
				PathPoint.IsThrough = true;
			}
			else PathPoint.IsThrough = false;

			AddLaneArray.Add(PathPoint);
		}

		FLanePoint BendingPoint;
		BendingPoint.Coordination = BendingCoord;
		BendingPoint.IsStation = false;
		BendingPoint.IsBendingPoint = true;
		if (GridManagerRef->GetGridCellDataByPoint(BendingCoord.X, BendingCoord.Y).StationInfo == GridStationStructure::Station) {
			BendingPoint.IsThrough = true;
		}
		else BendingPoint.IsThrough = false;

		AddLaneArray.Add(BendingPoint);

		for (const FIntPoint& Point : PathFromBending)
		{
			FLanePoint PathPoint;
			PathPoint.Coordination = Point;
			PathPoint.IsStation = false;
			PathPoint.IsBendingPoint = false;
			if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
				PathPoint.IsThrough = true;
			}
			else PathPoint.IsThrough = false;

			AddLaneArray.Add(PathPoint);
		}

	}
	else {
		TArray<FIntPoint> PathToNext = GeneratePath(CurrentStation, NextStation);

		for (const FIntPoint& Point : PathToNext)
		{
			FLanePoint PathPoint;
			PathPoint.Coordination = Point;
			PathPoint.IsStation = false;
			PathPoint.IsBendingPoint = false;
			if (GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station) {
				PathPoint.IsThrough = true;
			}
			else PathPoint.IsThrough = false;

			AddLaneArray.Add(PathPoint);
		}
	}

	FLanePoint NewLanePoint;
	NewLanePoint.Coordination = NewPoint;
	NewLanePoint.IsStation = true;
	NewLanePoint.IsBendingPoint = true;
	NewLanePoint.IsThrough = false;

	AddLaneArray.Add(NewLanePoint);

	int32 legacyNum = RLaneArray.Num() -1;
	RLaneArray.Insert(AddLaneArray, RLaneArray.Num());

	//Add LaneLoc
	if (!GridManagerRef) {
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is not valid."));
		return;
	}
	for (const FLanePoint& Point : AddLaneArray) {
		FVector VectorLocation = PointToLocation(Point.Coordination);
		RLaneLocation.Add(VectorLocation);
	}

	//Set Spline Again
//	Spline->SetSplinePoints(RLaneLocation, ESplineCoordinateSpace::World, true);
	SetLaneSpline(Spline);

	//[] Add Spline Mesh From End to New

	FVector StartPos;
	FVector StartTangent;
	FVector EndPos;
	FVector EndTangent;

	float Length;
	float ClampedLength;

	int32 NumSplinePoints = Spline->GetNumberOfSplinePoints();

//	EndPos, EndTangent
	EndPos = ((Spline->GetLocationAtSplinePoint(legacyNum, ESplineCoordinateSpace::Local)+Spline->GetLocationAtSplinePoint(legacyNum - 1, ESplineCoordinateSpace::Local))/2.0f);
	EndTangent = Spline->GetTangentAtSplinePoint(legacyNum - 1, ESplineCoordinateSpace::Local);

	for (int32 i = legacyNum; i < NumSplinePoints; i++) {
		//Set Points between
		if(i==NumSplinePoints -1){
			/* --- Back Mesh Only --- */
			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			StartTangent = EndTangent;
			EndPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);

			//Add&Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			SetSplineMeshComponent(SplineMeshComponent, RSplineMesh);
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}
		else{
			/* --- Back Mesh --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			EndPos = Spline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
			StartTangent = EndTangent;

			//Set Spline Mesh Component (mesh)
			USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
			else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
			RLaneArray[i].MeshArray.Add(SplineMeshComponent);

			/* --- Middle Mesh (if there is any) --- */
			if (RLaneArray[i].IsBendingPoint) {
				// Set Start/End Pos/Tangent
				StartPos = EndPos;
				EndTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

				//Set Spline Mesh Component (mesh)
				SplineMeshComponent = NewObject<USplineMeshComponent>(this);
				if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
				else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
				SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
				SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);
				RLaneArray[i].MeshArray.Add(SplineMeshComponent);
			}

			/* --- Front Mesh --- */

			// Set Start/End Pos/Tangent
			StartPos = EndPos;
			StartTangent = EndTangent;
			if (RLaneArray[i].IsBendingPoint) {
				StartTangent = Spline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
				Length = EndTangent.Size();
				ClampedLength = FMath::Clamp(Length, 0.0f, RSectionLength);
				StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
			}
			EndPos = ((StartPos + Spline->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local)) / 2.0f);
			EndTangent = StartTangent;

			//Set Spline Mesh Component (mesh)
			SplineMeshComponent = NewObject<USplineMeshComponent>(this);
			if (RLaneArray[i].IsThrough) SetSplineMeshComponent(SplineMeshComponent, RThroughMesh);
			else { SetSplineMeshComponent(SplineMeshComponent, RSplineMesh); }
			SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
			SplineMeshComponent->AttachToComponent(Spline, FAttachmentTransformRules::KeepWorldTransform);

			RLaneArray[i].MeshArray.Add(SplineMeshComponent);
		}
	}
}