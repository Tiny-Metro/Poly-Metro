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
#include "Lane/LaneSaveGame.h"
#include "SaveSystem/TMSaveManager.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <Kismet/GameplayStatics.h>
#include "Lane/LaneManager.h"
#include "Statistics/StatisticsManager.h"
#include "Algo/Reverse.h"

// Sets default values
ALane::ALane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Used in SpawnTrain function
	TrainClass = ConstructorHelpers::FObjectFinder<UClass>(TEXT("Class'/Game/Train/BP_Train.BP_Train_C'")).Object;

	LaneMaterial.AddUnique(
		ConstructorHelpers::FObjectFinder<UMaterial>(*LaneDefaultMaterialPath).Object
	);

	for (auto& i : RemoveLanePath) {
		RemoveLaneMaterial.AddUnique(ConstructorHelpers::FObjectFinder<UMaterial>(*i).Object);
	}
	if (!GridManagerRef)
	{
		GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	}

	OnPopHandleCalled.AddDynamic(this, &ALane::OnOtherLanePopHandleCalled);
	CurrentlyPoppingLane = false;
	InitLaneSpline();
	InitHandles();
}


// Called when the game starts or when spawned
void ALane::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	StationManagerRef = GameMode->GetStationManager();

	TrainManagerRef = GameMode->GetTrainManager();

	BTMangerREF = GameMode->GetBridgeTunnelManager();

	LaneManagerRef = GameMode->GetLaneManager();

	SaveManagerRef = GameMode->GetSaveManager();

	StatisticsManagerRef = GameMode->GetStatisticsManager();

	TinyMetroPlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	SaveManagerRef->SaveTask.AddDynamic(this, &ALane::Save);

	if (HasSaveFile) 
	{
		SpawnLaneMesh();
	}
}

// Called every frame
void ALane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].AverageComplain = GetStationComplainAverage();

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


FOnPopHandleCalled ALane::OnPopHandleCalled;

void ALane::OnOtherLanePopHandleCalled(bool IsUp)
{
//		UE_LOG(LogTemp, Warning, TEXT("OnOtherLanePopHandleCalled    Lane ID = %d"), LaneId);
	if (CurrentlyPoppingLane) {
//		UE_LOG(LogTemp, Warning, TEXT("OnOtherLanePopHandleCalled: IGNORED Lane ID = %d"), LaneId);
		return;  // Ignore if this lane is the one that's currently popping the handle
	}

	if (IsUp)
	{
//		UE_LOG(LogTemp, Warning, TEXT("OnOtherLanePopHandleCalled: GO down Lane ID = %d"), LaneId);
		PopHandle(false);
	}
}

void ALane::PopHandle(bool IsUp)
{
	if (!IsValid(StartHandle) || !IsValid(EndHandle))
	{
		UE_LOG(LogTemp, Warning, TEXT("PopHandle : NO Valid Handle - Lane ID = %d"), LaneId);
		return;
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("PopHandle : YES Valid Handle - Lane ID = %d"), LaneId);
	}

	FVector ChangedStartLoc = StartHandle->GetComponentLocation();
	FVector ChangedEndLoc = EndHandle->GetComponentLocation();
	float TargetZ = 1;
	if (IsUp)
	{
		TargetZ = 2;
		CurrentlyPoppingLane = true;
	//	UE_LOG(LogTemp, Warning, TEXT("PopHandle: GO down Lane ID = %d"), LaneId);
	}
	else 
	{
		CurrentlyPoppingLane = false;
	}
	ChangedStartLoc = FVector(ChangedStartLoc.X, ChangedStartLoc.Y, TargetZ);
	ChangedEndLoc = FVector(ChangedEndLoc.X, ChangedEndLoc.Y, TargetZ);
	StartHandle->SetWorldLocation(ChangedStartLoc);
	EndHandle->SetWorldLocation(ChangedEndLoc);

	if (IsUp) 
	{
		OnPopHandleCalled.Broadcast(IsUp);
		CurrentlyPoppingLane = false;
	}
}


void ALane::SetLaneId(int _LaneId)
{
	LaneId = _LaneId;
}

void ALane::InitLaneMaterial(TArray<UMaterial*> Materials) {
	LaneMaterial.Append(Materials);
}
void ALane::InitHandles()
{
	EndHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EndHandle"));
	StartHandle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StartHandle"));

	//set Staticmesh of it
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(*LanedHandleMeshPath);

	if (MeshAsset.Succeeded() )
	{
		EndHandle->SetStaticMesh(MeshAsset.Object);
		StartHandle->SetStaticMesh(MeshAsset.Object);
	}
}

bool ALane::GetIsCircularLine() const
{
	return IsCircularLine;
}

void ALane::SetIsCircularLine(bool _Circular)
{
	IsCircularLine = _Circular;

	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].IsCircularLane = _Circular;
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
	LaneDirection result = LaneDirection::Vertical;
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

void ALane::StopHandleEditing()
{
	if (IsStartHandleEditing)
	{
		IsStartHandleEditing = false;
	}

	if (IsEndHandleEditing)
	{
		IsEndHandleEditing = false;
	}
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
void ALane::RemoveLane_Implementation() {}

void ALane::ExtendLane_Implementation(AStation* Station) {}

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

void ALane::MarkLaneToRemoveFromStart(int32 Index)
{
	int32 tmpIndex = 0;
	int32 lastIndex;
	while (tmpIndex <= Index) {

		//Lane Point
		for (int32 i = 0; i < LaneArray.Num(); i++)
		{
			ChangeRemoveMaterialAtIndex(i);
			lastIndex = i;
			if (i != 0 && LaneArray[i].IsStation) { break; }
		}

		tmpIndex++;
	}

	LaneArray[lastIndex].MeshArray.Last()->SetMaterial(0, MeshMaterial);
}

void ALane::MarkLaneToRemoveFromEnd(int32 Index, int32 ExStationNum)
{
	int32 tmpIndex = ExStationNum - 1;
	int32 lastIndex;

	while (tmpIndex >= Index) {
		for (int32 i = LaneArray.Num() - 1; i >= 0; --i)
		{
			ChangeRemoveMaterialAtIndex(i);
			lastIndex = i;

			if (i != LaneArray.Num() - 1 && LaneArray[i].IsStation) { break; }
		}

		tmpIndex--;
	}
	LaneArray[lastIndex].MeshArray[0]->SetMaterial(0, MeshMaterial);

}

void ALane::MarkLaneToClear()
{
	for (int32 i = 0; i < LaneArray.Num(); i++) {
		ChangeRemoveMaterialAtIndex(i);
	}
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

					if (!TrainClass) {
						TrainClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Class'/Game/Train/BP_Train.BP_Train_C'")));
					}

					FTransform SpawnTransform;

					FVector SpawnLocation = StationPoint[0]->GetCurrentGridCellData().WorldLocation;
					SpawnLocation = { SpawnLocation.X, SpawnLocation.Y, 20 };

					SpawnTransform.SetLocation(SpawnLocation);

					ATrain* Train = Cast<ATrain>(GetWorld()->SpawnActor<AActor>(TrainClass, SpawnTransform));


					Train->ServiceStart(Train->GetActorLocation(), this, Destination);
					
					}),
				1.0f,
				false,
				0.6f
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

// ========= Setting Lane Array ======== //
void ALane::SetLaneArray(const TArray<class AStation*>& NewStationPoint) {
	//check if station is valid
	if (!IsStationsValid(NewStationPoint)) return;

	//check if lane points are valid
	TArray<FLanePoint> PreLaneArray;
	GetLaneArray(NewStationPoint, PreLaneArray);


	// check if tunnel&bridge - and can be used
	if (!IsBuildble(PreLaneArray))
	{	
		Destroy();
		return;
	}

	// Get Area that requires Connectors
	TArray<TArray<FIntPoint>> TunnelArea = GetConnectorArea(PreLaneArray, GridType::Hill);
	TArray<TArray<FIntPoint>> BridgeArea = GetConnectorArea(PreLaneArray, GridType::Water);

	// if it is possible, bulid it
	for (int i = 0; i < BridgeArea.Num(); i++) 
	{		
		BTMangerREF->BuildConnector(ConnectorType::Bridge, BridgeArea[i], LaneId);
		UpdateUsingConnector();
	}
	for (int i = 0; i < TunnelArea.Num(); i++)
	{
		BTMangerREF->BuildConnector(ConnectorType::Tunnel, TunnelArea[i], LaneId);
		UpdateUsingConnector();
	}

	// Done
	StationPoint = NewStationPoint;
	ClearLanePoint();
	LaneArray = PreLaneArray;

}

void ALane::GetLaneArray(const TArray<class AStation*>& NewStationPoint, TArray<FLanePoint>& PreLaneArray)
{
	//check if lane points are valid
	PreLaneArray.Empty();
	int32 NumStations = NewStationPoint.Num();

	for (int32 i = 0; i < NumStations; i++)
	{
		TArray<FLanePoint> LaneBlock;
		LaneBlock.Empty();

		if (i == NumStations - 1)
		{
			AStation* CurrentStationPtr = NewStationPoint[i];
			FIntPoint CurrentStation = CurrentStationPtr->GetCurrentGridCellData().WorldCoordination;
			FLanePoint CurrentLanePoint;
			AddLanePoint(CurrentStation, true, true, LaneBlock);

			PreLaneArray.Append(LaneBlock);
			break;
		}

		AStation* CurrentStationPtr = NewStationPoint[i];
		AStation* NextStationPtr = (i < NumStations - 1) ? NewStationPoint[i + 1] : nullptr;

		LaneBlock = GetLanePath(CurrentStationPtr, NextStationPtr);
		int32 lastIndex = LaneBlock.Num() - 1;
		LaneBlock.RemoveAt(lastIndex);
		PreLaneArray.Append(LaneBlock);
	}

}
void GetLaneArrays(FIntPoint StartStationCoord, FIntPoint AddedStationCoord, TArray<FLanePoint>& PreLaneArray) 
{
	//check if lane points are valid
	PreLaneArray.Empty();

	TArray<FLanePoint> LaneBlock;
	LaneBlock.Empty();


//	LaneBlock = GetLanePathByPoint(StartStationCoord, AddedStationCoord);

	PreLaneArray.Append(LaneBlock);
}


bool ALane::CheckExtendable(FIntPoint StartingStationCoordinate, FIntPoint AddedStationCoordinate)
{
	UE_LOG(LogTemp, Warning, TEXT("CheckExtendalbe"))
	//Create Coord Array for path
	TArray<FLanePoint> LaneBlock;

	FIntPoint StartStationCoord = StartingStationCoordinate;
	FIntPoint NextStationCoord = AddedStationCoordinate;

	FIntPoint Diff = NextStationCoord - StartStationCoord;

	bool IsBending = HasBendingPoint(Diff);


	if (IsBending)
	{
		FIntPoint BendingCoord = FindBendingPoint(StartStationCoord, NextStationCoord);
		int32 Position = LaneManagerRef->GetPosition(StartStationCoord, BendingCoord);

		AddLanePoint(StartStationCoord, true, true, LaneBlock, Position);

		// Generate paths
		TArray<FIntPoint> PathToBending = GeneratePath(StartStationCoord, BendingCoord);
		TArray<FIntPoint> PathFromBending = GeneratePath(BendingCoord, NextStationCoord);

		// Add PathToBending points to LaneArray
		for (const FIntPoint& Point : PathToBending)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		// Add BendingPoint to LaneArray
		Position = LaneManagerRef->GetPosition(BendingCoord, NextStationCoord);
		AddLanePoint(BendingCoord, false, true, LaneBlock, Position);

		// Add PathFromBending points to LaneArray
		for (const FIntPoint& Point : PathFromBending)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		AddLanePoint(NextStationCoord, true, true, LaneBlock);

	}
	else
	{
		int32 Position = LaneManagerRef->GetPosition(StartStationCoord, NextStationCoord);

		AddLanePoint(StartStationCoord, true, true, LaneBlock, Position);

		TArray<FIntPoint> PathToNext = GeneratePath(StartStationCoord, NextStationCoord);

		// Add PathToNext points to LaneArray
		for (const FIntPoint& Point : PathToNext)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		AddLanePoint(NextStationCoord, true, true, LaneBlock);

	}


	//Check Required Items
	return IsBuildble(LaneBlock);

}

TArray<TArray<FIntPoint>> ALane::GetConnectorArea(TArray<FLanePoint>& LaneBlock, GridType type)
{
	TArray<TArray<FIntPoint>> TargetArea;
	TargetArea.Empty();

	ConnectorType TargetConnector = GetConnectorType(type);
	if (TargetConnector == ConnectorType::None)
	{
		return TargetArea;
	}

	TArray<FIntPoint> TargetPoints;
	TArray<int> TargetIndex;

	for (int i = 0; i < LaneBlock.Num(); i++)
	{
		FIntPoint Coord = LaneBlock[i].Coordination;

		FGridCellData GridCellData = GridManagerRef->GetGridCellDataByPoint(Coord.X, Coord.Y);
		GridType GridCellType = GridCellData.GridType;
		if (GridCellType == type)
		{
			if (TargetIndex.IsEmpty())
			{
				TargetPoints.Add(LaneBlock[i - 1].Coordination);
			}
			if (TargetIndex.Num() >= 1 && TargetIndex.Last() + 1 != i)
			{
				TargetPoints.Add(LaneBlock[TargetIndex.Last() + 1].Coordination);
				TargetPoints.Add(LaneBlock[i - 1].Coordination);
			}
			TargetIndex.Add(i);
			TargetPoints.Add(Coord);
		}
	}

	if (TargetIndex.Num() > 0)
	{
		TargetPoints.Add(LaneBlock[TargetIndex.Last() + 1].Coordination);
	}


	SetArea(TargetPoints, TargetArea);

	return TargetArea;
}

int32 ALane::GetRequiredConnector(TArray<TArray<FIntPoint>>& AreaArray, GridType type)
{

	ConnectorType TargetConnector = GetConnectorType(type);
	if (TargetConnector == ConnectorType::None)
	{
		return 0;
	}

	int32 RequiredNum = 0;


	for(int i = 0; i < AreaArray.Num(); i++)
	{
		bool IsExist = BTMangerREF->IsConnectorExist(TargetConnector, AreaArray[i]);
		if (!IsExist)
		{
			RequiredNum++;
		}
	}
	return RequiredNum;
}

ConnectorType ALane::GetConnectorType(GridType Type)
{
	ConnectorType TargetConnector;
	switch (Type)
	{
	case GridType::Water:
		TargetConnector = ConnectorType::Bridge;
		break;
	case GridType::Hill:
		TargetConnector = ConnectorType::Tunnel;
		break;
	default:
		TargetConnector = ConnectorType::None;
		break;
	}
	return TargetConnector;
}

TArray<TArray<FIntPoint>> ALane::GetArea(const TArray<FLanePoint>& LaneBlock, GridType Type) {
	TArray<FIntPoint> targetPoints;

	TArray<int> targetIndex;

	for (int i = 0; i < LaneBlock.Num(); i++)
	{

		FIntPoint Coord = LaneBlock[i].Coordination;

		FGridCellData GridCellData = GridManagerRef->GetGridCellDataByPoint(Coord.X, Coord.Y);

		if (GridCellData.GridType == Type) {
			if (targetIndex.IsEmpty())
			{
				targetPoints.Add(LaneBlock[i - 1].Coordination);
			}
			if (targetIndex.Num() >= 1 && targetIndex.Last() + 1 != i)
			{
				targetPoints.Add(LaneBlock[targetIndex.Last() + 1].Coordination);
				targetPoints.Add(LaneBlock[i - 1].Coordination);
			}
			targetIndex.Add(i);
			targetPoints.Add(Coord);
		}


	}

	if (targetIndex.Num() > 0)// && targetIndex.Last() + 1 < LaneBlock.Num())
	{
		targetPoints.Add(LaneBlock[targetIndex.Last() + 1].Coordination);
	}


	/////////

	TArray<TArray<FIntPoint>> target;
	int32 NumPoints = targetPoints.Num();

	if (NumPoints == 0)
	{
		target.Empty();
		return target;
	}

	TArray<FIntPoint> Area;
	Area.Add(targetPoints[0]);

	for (int32 i = 1; i < NumPoints; i++)
	{
		const FIntPoint& CurrentCoord = targetPoints[i];
		const FIntPoint& PrevCoord = targetPoints[i - 1];

		int32 XDiff = FMath::Abs(CurrentCoord.X - PrevCoord.X);
		int32 YDiff = FMath::Abs(CurrentCoord.Y - PrevCoord.Y);

		if (XDiff <= 1 && YDiff <= 1)
		{
			// Coordinates are within the range of (-1, -1) and (1, 1), indicating continuity
			Area.Add(CurrentCoord);
		}
		else
		{
			// Coordinates are not continuous
			target.AddUnique(Area);
			Area.Empty();
			Area.Add(CurrentCoord);
		}
	}

	target.Add(Area);


	return target;
}
void ALane::DisconnectBT(TArray<TArray<FIntPoint>> Area, GridType Type) {

	ConnectorType targetType = GetConnectorType(Type);
	if (targetType == ConnectorType::None) { return; }

	for (int i = 0; i < Area.Num(); i++)
	{
		BTMangerREF->DisconnectByInfo(targetType, Area[i], LaneId);
	}
}

void ALane::ConnectBT(TArray<TArray<FIntPoint>> Area, GridType Type) {

	ConnectorType targetType = GetConnectorType(Type);
	if (targetType == ConnectorType::None) 
	{
		return;
	}

	for (int i = 0; i < Area.Num(); i++)
	{
		BTMangerREF->BuildConnector(targetType, Area[i], LaneId);
		UpdateUsingConnector();
	}
}

void ALane::SetArea(const TArray<FIntPoint>& Points, TArray<TArray<FIntPoint>>& AreaArray)
{
	int32 NumPoints = Points.Num();

	if (NumPoints == 0)
	{
		return;
	}

	TArray<FIntPoint> Area;
	Area.Add(Points[0]);

	for (int32 i = 1; i < NumPoints; i++)
	{
		const FIntPoint& CurrentCoord = Points[i];
		const FIntPoint& PrevCoord = Points[i - 1];

		int32 XDiff = FMath::Abs(CurrentCoord.X - PrevCoord.X);
		int32 YDiff = FMath::Abs(CurrentCoord.Y - PrevCoord.Y);

		if (XDiff <= 1 && YDiff <= 1)
		{
			// Coordinates are within the range of (-1, -1) and (1, 1), indicating continuity
			Area.Add(CurrentCoord);
		}
		else
		{
			// Coordinates are not continuous
			AreaArray.AddUnique(Area);
			Area.Empty();
			Area.Add(CurrentCoord);
		}
	}

	AreaArray.Add(Area);
}

bool ALane::IsBuildble(TArray<FLanePoint>& LaneBlock)
{
	TArray<TArray<FIntPoint>> TunnelArea = GetConnectorArea(LaneBlock, GridType::Hill);
	TArray<TArray<FIntPoint>> BridgeArea = GetConnectorArea(LaneBlock, GridType::Water);

	int32 RequiredTunnel = GetRequiredConnector(TunnelArea, GridType::Hill);
	int32 RequiredBridge = GetRequiredConnector(TunnelArea, GridType::Water);

	if (RequiredBridge> TinyMetroPlayerState->GetValidBridgeCount() )
	{
		return false;
	}
	if (RequiredTunnel > TinyMetroPlayerState->GetValidTunnelCount()) 
	{
		return false;
	}

	return true;
}

void ALane::Save()
{
	ULaneSaveGame* tmp = Cast<ULaneSaveGame>(UGameplayStatics::CreateSaveGameObject(ULaneSaveGame::StaticClass()));

	tmp->IsCircularLane = IsCircularLine;

	//StationPoint Save
	for (const auto& i : StationPoint)
	{
		tmp->StationPoint.Add(i->GetStationId());
	}

	//PointArray Save
	for (const auto& i : PointArray)
	{
		tmp->PointArray.Add(i);
	}

	for (const auto& i : LaneLocation)
	{
		tmp->LaneLocation.Add(i);
	}

	for (const auto& i : LaneArray)
	{
		FLanePoint LanePoint;

		LanePoint.Coordination = i.Coordination;
		LanePoint.IsStation = i.IsStation;
		LanePoint.IsBendingPoint = i.IsBendingPoint;
		LanePoint.IsThrough = i.IsThrough;
		LanePoint.LanePosition = i.LanePosition;
		LanePoint.LaneType = i.LaneType;
		LanePoint.LaneDirection = i.LaneDirection;
		LanePoint.WillBeRemoved = i.WillBeRemoved;

		tmp->LaneArray.Add(LanePoint);
	}
	
	//Delay Removing
	tmp->DoesStationsToBeRemovedAtStart = DoesStationsToBeRemovedAtStart;
	tmp->DoesStationsToBeRemovedAtEnd = DoesStationsToBeRemovedAtEnd;
	tmp->DoesLaneToBeRemoved = DoesLaneToBeRemoved;
	
	for (const auto& i : StationsToBeRemovedAtStart)
	{
		tmp->StationsToBeRemovedAtStart.Add(i->GetStationId());
	}

	for (const auto& i : StationsToBeRemovedAtEnd)
	{
		tmp->StationsToBeRemovedAtEnd.Add(i->GetStationId());
	}

	for (const auto& i : StationPointBeforeRemovedStart)
	{
		tmp->StationPointBeforeRemovedStart.Add(i->GetStationId());
	}

	for (const auto& i : StationPointBeforeRemovedEnd)
	{
		tmp->StationPointBeforeRemovedEnd.Add(i->GetStationId());
	}

	SaveManagerRef->Save(tmp, SaveActorType::Lane, LaneId);
}

bool ALane::Load()
{
	if (!GameMode) {
		GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	if (!SaveManagerRef) {
		SaveManagerRef = GameMode->GetSaveManager();
	}
	if (!StationManagerRef)
	{
		StationManagerRef = GameMode->GetStationManager();
	}
	if (!GridManagerRef)
	{
		GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	}

	SetMeshMaterial();
	SetHandleMaterial();
	//Load Lane Data
	ULaneSaveGame* tmp = Cast<ULaneSaveGame>(SaveManagerRef->Load(SaveActorType::Lane, LaneId));

	if (!IsValid(tmp)) {
		UE_LOG(LogTemp, Warning, TEXT("Lane %d SaveGame is not valid"), LaneId);
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("Lane %d is loaded"), LaneId);

	
	IsCircularLine = tmp->IsCircularLane;

	//StationPoint Save
	for (const auto& i : tmp->StationPoint)
	{
		AStation* Station = StationManagerRef->GetStationById(i);
		StationPoint.Add(StationManagerRef->GetStationById(i));

		Station->SetActivate(true);
	}

	//AdjList Load
	for (int i = 0; i < StationPoint.Num()-1 ; i++)
	{
		AddAdjListDistance(StationPoint[i], StationPoint[i+1]);
	}

	//SetLane
	for (int i = 0; i < StationPoint.Num(); i++)
	{
		if (i == StationPoint.Num() - 1)
		{
			if (IsCircularLine)
			{
				break;
			}
			else
			{
				StationPoint[i]->SetLanes(LaneId);
			}
		}
		else
		{
			StationPoint[i]->SetLanes(LaneId);
		}
		
	}



	//PointArray Save
	for (const auto& i : tmp->PointArray)
	{
		PointArray.Add(i);
	}

	for (const auto& i : tmp->LaneLocation)
	{
		LaneLocation.Add(i);
	}

	for (const auto& i : tmp->LaneArray)
	{
		FLanePoint LanePoint;

		LanePoint.Coordination = i.Coordination;
		LanePoint.IsStation = i.IsStation;
		LanePoint.IsBendingPoint = i.IsBendingPoint;
		LanePoint.IsThrough = i.IsThrough;
		LanePoint.LanePosition = i.LanePosition;
		LanePoint.LaneType = i.LaneType;
		LanePoint.LaneDirection = i.LaneDirection;
		LanePoint.WillBeRemoved = i.WillBeRemoved;

		LaneArray.Add(LanePoint);
	}

	//Delay Removing
	DoesStationsToBeRemovedAtStart = tmp->DoesStationsToBeRemovedAtStart;
	DoesStationsToBeRemovedAtEnd = tmp->DoesStationsToBeRemovedAtEnd;
	DoesLaneToBeRemoved = tmp->DoesLaneToBeRemoved;

	for (const auto& i : tmp->StationsToBeRemovedAtStart)
	{
		StationsToBeRemovedAtStart.Add(StationManagerRef->GetStationById(i));
	}

	for (const auto& i : tmp->StationsToBeRemovedAtEnd)
	{
		StationsToBeRemovedAtEnd.Add(StationManagerRef->GetStationById(i));
	}

	for (const auto& i : tmp->StationPointBeforeRemovedStart)
	{
		StationPointBeforeRemovedStart.Add(StationManagerRef->GetStationById(i));
	}

	for (const auto& i : tmp->StationPointBeforeRemovedEnd)
	{
		StationPointBeforeRemovedEnd.Add(StationManagerRef->GetStationById(i));
	}

	return true;
}

bool ALane::HasBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation) {
	FIntPoint Diff = NextStation - CurrentStation;

	if (Diff.X == 0) return false;
	if (Diff.Y == 0) return false;
	if (FMath::Abs(Diff.X) == FMath::Abs(Diff.Y)) return false;
	return true;
}

bool ALane::HasBendingPoint(FIntPoint Diff) {
	return Diff.X != 0 && Diff.Y != 0 && FMath::Abs(Diff.X) != FMath::Abs(Diff.Y);
}

FIntPoint ALane::FindBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation) {
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


// ======= Sets Lane Points' world Locations and Spline Point's World Location ========//
void ALane::UpdateLocationAndSpline()
{
	SetLaneLocation();
	SetLaneSpline();
}

void ALane::SetLaneLocation() {

	if (!GridManagerRef){
		UE_LOG(LogTemp, Warning, TEXT("SetLaneLocation: GridManagerRef is not valid."));
		return;
	}

	LaneLocation.Empty(); 
	float Offset = 100;

	for (int i = 0; i < LaneArray.Num(); i++)
	{
		FLanePoint& CurrentPoint = LaneArray[i];
		CurrentPoint.SplineIndex.Empty();
		FVector PointVector = PointToLocation(CurrentPoint.Coordination);
		FVector LineDirection;

		float off = CalculateOffset(CurrentPoint.LanePosition);
		FVector VectorLocation = PointVector;

		if (i == 0)
		{
			FLanePoint FrontPoint = LaneArray[i + 1];
			FVector FrontVector = PointToLocation(FrontPoint.Coordination);
			LineDirection = CalculateLineDirection(FrontVector, PointVector);
			VectorLocation += CalculatePerpendicular(LineDirection, Offset, off);
		}
		else if (i == LaneArray.Num() - 1)
		{
			FLanePoint BackPoint = LaneArray[i - 1];
			FVector BackVector = PointToLocation(BackPoint.Coordination);
			LineDirection = CalculateLineDirection(PointVector, BackVector);
			off = CalculateOffset(BackPoint.LanePosition);
			VectorLocation += CalculatePerpendicular(LineDirection, Offset, off);
		}
		else
		{
			if (CurrentPoint.IsBendingPoint)
			{
				FLanePoint BackPoint = LaneArray[i - 1];
				FLanePoint FrontPoint = LaneArray[i + 1];

				FVector BackVector = PointToLocation(BackPoint.Coordination);
				FVector FrontVector = PointToLocation(FrontPoint.Coordination);

				FVector BackDirection = CalculateLineDirection(PointVector, BackVector);
				FVector FrontDirection = CalculateLineDirection(FrontVector, PointVector);

				FVector BackPerpendicularVector = FVector::CrossProduct(BackDirection, FVector::UpVector);
				BackPerpendicularVector= ChangePerpendicularToStandard(BackPerpendicularVector);

				FVector FrontPerpendicularVector = FVector::CrossProduct(FrontDirection, FVector::UpVector);
				FrontPerpendicularVector = ChangePerpendicularToStandard(FrontPerpendicularVector);
				
				int32 BackOff = CalculateOffset(BackPoint.LanePosition);
				int32 FrontOff = CalculateOffset(CurrentPoint.LanePosition);

				FVector BackVectorLocation = PointVector + BackPerpendicularVector * Offset * BackOff;
				FVector FrontVectorLocation = PointVector + FrontPerpendicularVector * Offset * FrontOff;

				VectorLocation = (LineIntersection(BackVector + BackPerpendicularVector * Offset * BackOff, BackVectorLocation, FrontVectorLocation, FrontVector + FrontPerpendicularVector * Offset * FrontOff));
				if (BackPerpendicularVector == FrontPerpendicularVector && BackOff != FrontOff)
				{
					LaneLocation.Add(BackVectorLocation);
					CurrentPoint.SplineIndex.Add(LaneLocation.Num() - 1);
					UE_LOG(LogTemp, Warning, TEXT("SetLaneLocation: ADD MORE"));

					VectorLocation = FrontVectorLocation;
				}
			}
			else
			{
				FLanePoint FrontPoint = LaneArray[i + 1];
				FVector FrontVector = PointToLocation(FrontPoint.Coordination);
				LineDirection = CalculateLineDirection(FrontVector, PointVector);
				VectorLocation += CalculatePerpendicular(LineDirection, Offset, off);
			}
		}
		LaneLocation.Add(VectorLocation);
		CurrentPoint.SplineIndex.AddUnique(LaneLocation.Num() - 1);
		for (int32 j = 0; j < CurrentPoint.SplineIndex.Num(); j++)
		{
			UE_LOG(LogTemp, Warning, TEXT("SetLaneLocation: %d (%d)"), i, CurrentPoint.SplineIndex[j]);
		}
	}

	for (int i = 0; i < LaneArray.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetLaneLocation: %d -> %d"), i, LaneArray[i].SplineIndex.Num());
	}

}

void ALane::SetLaneSpline(){
	LaneSpline->SetSplinePoints(LaneLocation, ESplineCoordinateSpace::World,true);

	for (int32 i = 0; i < LaneLocation.Num(); i++) {
		LaneSpline->SetSplinePointType(i, ESplinePointType::Linear, true);
	}
	UE_LOG(LogTemp, Warning, TEXT("SetLaneSpline: %d"), LaneSpline->GetNumberOfSplinePoints());
}

// Heloper Function for 'SetLaneLocation'
FVector ALane::LineIntersection(FVector A, FVector B, FVector C, FVector D)
{
	float a1 = B.Y - A.Y;
	float b1 = A.X - B.X;
	float c1 = a1 * (A.X) + b1 * (A.Y);

	float a2 = D.Y - C.Y;
	float b2 = C.X - D.X;
	float c2 = a2 * (C.X) + b2 * (C.Y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel
		return B;
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		return FVector(x, y, B.Z); // z-coordinate is zero as this is in 2D
	}
}
float ALane::CalculateOffset(int32 LanePosition)
{
	return (LanePosition % 2 == 0) ? LanePosition / 2 : ((LanePosition + 1) / 2) * -1;
}
FVector ALane::CalculateLineDirection(FVector Vector1, FVector Vector2)
{
	return (Vector1 - Vector2).GetSafeNormal();
}
FVector ALane::CalculatePerpendicular(FVector LineDirection, float Offset, float off)
{
	FVector PerpendicularVector = FVector::CrossProduct(LineDirection, FVector::UpVector);

	PerpendicularVector = ChangePerpendicularToStandard(PerpendicularVector);

	return PerpendicularVector * Offset * off;
}

FVector ALane::ChangePerpendicularToStandard(FVector Perpendicular) 
{
	Perpendicular.Normalize();

	FVector y = FVector(0, 1, 0);
	y.Normalize();

	FVector x = FVector(1, 0, 0);
	x.Normalize();

	FVector xy = FVector(1, 1, 0);
	xy.Normalize();

	FVector xyMinous = FVector(-1, 1, 0);
	xyMinous.Normalize();

	constexpr float epsilon = 0.0001f;
	FVector targetStandard = Perpendicular;
	if ((Perpendicular - y).Size() < epsilon || (Perpendicular - (-y)).Size() < epsilon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CalculatePerPendicular: Y"));
		targetStandard = y;
	}

	if ((Perpendicular - x).Size() < epsilon || (Perpendicular - (-x)).Size() < epsilon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CalculatePerPendicular: X"));
		targetStandard = x;

	}

	if ((Perpendicular - xy).Size() < epsilon || (Perpendicular - (-xy)).Size() < epsilon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CalculatePerPendicular: XY"));
		targetStandard = xy;

	}

	if ((Perpendicular - xyMinous).Size() < epsilon || (Perpendicular - (-xyMinous)).Size() < epsilon)
	{
		UE_LOG(LogTemp, Warning, TEXT("CalculatePerPendicular: xyMinous"));
		targetStandard = xyMinous;

	}

	return targetStandard;
}

//=================================================================================================//

void ALane::AddLanePoint(const FIntPoint& Point, bool IsStation, bool IsBendingPoint, TArray<FLanePoint>& TargetArray) {
	FLanePoint LanePoint;
	LanePoint.Coordination = Point;
	LanePoint.IsStation = IsStation;
	LanePoint.IsBendingPoint = IsBendingPoint;
	LanePoint.WillBeRemoved = false;
	if (IsStation) {
		LanePoint.IsThrough = false;
	}
	else {
		LanePoint.IsThrough = GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station;
	}

	TargetArray.Add(LanePoint);
}
void ALane::AddLanePoint(const FIntPoint& Point, bool IsStation, bool IsBendingPoint, TArray<FLanePoint>& TargetArray, int32 Position) {
	FLanePoint LanePoint;
	LanePoint.Coordination = Point;
	LanePoint.IsStation = IsStation;
	LanePoint.IsBendingPoint = IsBendingPoint;
	LanePoint.LanePosition = Position;
	LanePoint.WillBeRemoved = false;

	if (IsStation) {
		LanePoint.IsThrough = false;
	}
	else {
		LanePoint.IsThrough = GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).StationInfo == GridStationStructure::Station;
	}

	TargetArray.Add(LanePoint);
}

FVector ALane::PointToLocation(const FIntPoint& Point) {
	return GridManagerRef->GetGridCellDataByPoint(Point.X, Point.Y).WorldLocation;
}

void ALane::ClearLanePoint() {
	// Clear the existing lane array if any
	for (int32 i = 0; i < LaneArray.Num(); i++) {
		ClearSplineMeshAt(i);
	}
	LaneArray.Empty();
}


void ALane::SetMeshMaterial() {
	MeshMaterial = LaneMaterial[LaneId];
	RemoveMeshMaterial = RemoveLaneMaterial[LaneId -1];
}

void ALane::SetHandleMaterial()
{
	StartHandle->SetMaterial(0, MeshMaterial);
	EndHandle->SetMaterial(0, MeshMaterial);
}

void ALane::SetHandleTransform()
{
	if (!GridManagerRef)
	{
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is Null"));
		return;
	}
	if (LaneArray.IsValidIndex(0))
	{
	FVector StartLocation = PointToLocation(LaneArray[0].Coordination);
	FRotator StartRotator = (StartLocation - PointToLocation(LaneArray[1].Coordination)).Rotation();
	UE_LOG(LogTemp, Warning, TEXT("StartLocation X::%f , Y::%f, Z::%f"), StartLocation.X, StartLocation.Y, StartLocation.Z);
	StartHandle->SetWorldLocation(StartLocation);
	StartHandle->SetWorldRotation(StartRotator);

	FVector EndLocation = PointToLocation(LaneArray.Last(0).Coordination);
	FRotator EndRotator = (EndLocation - PointToLocation(LaneArray.Last(1).Coordination)).Rotation();
	EndHandle->SetWorldLocation(EndLocation);
	EndHandle->SetWorldRotation(EndRotator);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("SetHandleTransform : 0 index Invalid"));
		return;
	}
}

void ALane::ChangeRemoveMaterialAtIndex(int32 Index) // also make WillBeRemoved = true;
{
	LaneArray[Index].WillBeRemoved = true;
	for (USplineMeshComponent* mesh : LaneArray[Index].MeshArray) {

		UE_LOG(LogTemp, Warning, TEXT("MeshArray"));
		mesh->SetMaterial(0, RemoveMeshMaterial);
	}
}

void ALane::SetSplineMeshes(){

	//Check the input parameter is valid
	if (!LaneSpline)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid input parameters for R2SplineMeshComponent."));
		return;
	}

	int32 NumSplinePointsd = LaneSpline->GetNumberOfSplinePoints();

	SetMeshByIndex(0, NumSplinePointsd - 1);
	
	return;
}

void ALane::SetSplineMeshComponent(USplineMeshComponent* SplineMeshComponent, UStaticMesh* SplineMesh, int32 Index, int32 Pos) {
	SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	SplineMeshComponent->SetStaticMesh(SplineMesh);

	SplineMeshComponent->SetMobility(EComponentMobility::Movable);
	//Set Material
	SplineMeshComponent->SetMaterial(0, MeshMaterial);
	//Set Axis
	SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::X, false);

	SplineMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	SplineMeshComponent->RegisterComponent();

	// Set Location - the location doesn't represent where the meshes are - it represents where it belong and other infos
	SplineMeshComponent->SetWorldLocation(FVector(LaneId, Index, Pos));
	FVector vec = SplineMeshComponent->GetComponentLocation();
}

void ALane::SetMesh(UStaticMesh* Mesh, UStaticMesh* Through) {
	LaneMesh = Mesh;
	ThroughMesh = Through;
}

void ALane::ClearSplineMeshAt(int32 Index){
	if (!LaneArray.IsValidIndex(Index))
	{
		UE_LOG(LogTemp, Warning, TEXT("ClearSplineMeshAt : Invalid LaneArray Index"));
		return;
	}
	for (USplineMeshComponent* SplineMeshComponent : LaneArray[Index].MeshArray) {
		if (SplineMeshComponent) {
			SplineMeshComponent->DestroyComponent();
		}
	}
}

void ALane::RemoveLaneFromStart(int32 Index){

	// get preseverPOints for the BnT
	TArray<FLanePoint> PreserveLane;
	int32 tmpIndex = 0;
	int32 ind = 0;
	while (tmpIndex < Index +1)
	{
		PreserveLane.Add(LaneArray[ind]);
		UE_LOG(LogTemp, Warning, TEXT("!!elements : %d     %d"), LaneArray[ind].Coordination.X, LaneArray[ind].Coordination.Y);
		ind++;

		if (LaneArray[ind].IsStation) { tmpIndex++; }

	}
	PreserveLane.Add(LaneArray[ind]);

	//get area for the preserved points
	TArray<TArray<FIntPoint>> DeletedBridge = GetArea(PreserveLane, GridType::Water);
	DisconnectBT(DeletedBridge, GridType::Water);

	TArray<TArray<FIntPoint>> DeletedTunnel = GetArea(PreserveLane, GridType::Hill);
	DisconnectBT(DeletedTunnel, GridType::Hill);

	tmpIndex = 0;
	while (tmpIndex <= Index) {
		//StationPoint.RemoveAt(0);

		//Lane Point
		int count = 1;
		for (int32 i = 1; i < LaneArray.Num(); ++i)
		{
			if (LaneArray[i].IsStation) { break; }

			else {
				ClearSplineMeshAt(i);
				LaneArray.RemoveAt(i);
				--i; // Decrement index to account for removed element
				count++;
			}
		}

		ClearSplineMeshAt(0);
		LaneArray.RemoveAt(0);
		tmpIndex++;
	}

	UpdateLocationAndSpline();
	//Set StartPoint's Mesh Again
	ClearSplineMeshAt(0);

	FVector StartPos = LaneSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector EndPos = ((StartPos + LaneSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local)) / 2.0f);

	FVector StartTangent = LaneSpline->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector EndTangent;
	float Length = StartTangent.Size();
	float ClampedLength = FMath::Clamp(Length, 0.0f, SectionLength);
	StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
	EndTangent = StartTangent;

	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
	SetSplineMeshComponent(SplineMeshComponent, LaneMesh, 0, 3);
	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	SplineMeshComponent->AttachToComponent(LaneSpline, FAttachmentTransformRules::KeepWorldTransform);

	LaneArray[0].MeshArray.Add(SplineMeshComponent);


}

void ALane::RemoveLaneFromEnd(int32 Index, int32 ExStationNum){

	TArray<FLanePoint> PreserveLane;
	int32 tmpIndex = ExStationNum - 1;
	int32 ind = LaneArray.Num()-1;
	while (tmpIndex >= Index )
	{
		PreserveLane.Add(LaneArray[ind]);
		UE_LOG(LogTemp, Warning, TEXT("!!elements : %d     %d"), LaneArray[ind].Coordination.X, LaneArray[ind].Coordination.Y);
		ind--;

		if (LaneArray[ind].IsStation) { tmpIndex--; }

	}
	PreserveLane.Add(LaneArray[ind]);

	//get area for the preserved points
	TArray<TArray<FIntPoint>> DeletedBridge = GetArea(PreserveLane, GridType::Water);
	DisconnectBT(DeletedBridge, GridType::Water);

	TArray<TArray<FIntPoint>> DeletedTunnel = GetArea(PreserveLane, GridType::Hill);
	DisconnectBT(DeletedTunnel, GridType::Hill);



	//	int32 tmpIndex = StationPoint.Num() -1;
	tmpIndex = ExStationNum - 1;

	while (tmpIndex >= Index) {


		//Lane Point
		int count = 1;
		for (int32 i = LaneArray.Num() - 2; i >= 0; --i)
		{
			if (LaneArray[i].IsStation) { break; }
			else {
				ClearSplineMeshAt(i);
				LaneArray.RemoveAt(i);
				count++;
			}
		}

		ClearSplineMeshAt(LaneArray.Num() - 1);
		LaneArray.RemoveAt(LaneArray.Num() - 1);

		tmpIndex--;
	}

	UpdateLocationAndSpline();

	//Set EndPoint's Mesh Again
	int32 lastIndex = LaneArray.Num() - 1;
	ClearSplineMeshAt(lastIndex);

	FVector EndPos = LaneSpline->GetLocationAtSplinePoint(lastIndex, ESplineCoordinateSpace::Local);
	FVector StartPos = ((EndPos+ LaneSpline->GetLocationAtSplinePoint(lastIndex-1, ESplineCoordinateSpace::Local)) / 2.0f);

	FVector StartTangent = LaneSpline->GetTangentAtSplinePoint(lastIndex-1, ESplineCoordinateSpace::Local);
	FVector EndTangent;
	float Length = StartTangent.Size();
	float ClampedLength = FMath::Clamp(Length, 0.0f, SectionLength);
	StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
	EndTangent = StartTangent;

	//Add&Set Spline Mesh Component (mesh)
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
	SetSplineMeshComponent(SplineMeshComponent, LaneMesh, lastIndex, 1);
	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	SplineMeshComponent->AttachToComponent(LaneSpline, FAttachmentTransformRules::KeepWorldTransform);

	LaneArray[lastIndex].MeshArray.Add(SplineMeshComponent);
}

void ALane::ExtendStart(AStation* NewStation){

	ClearSplineMeshAt(0);

//Add Lane Array
	TArray<FLanePoint> AddLaneArray;

	FIntPoint NewPoint = NewStation->GetCurrentGridCellData().WorldCoordination;

	AStation* NextStation = StationPoint[1];

	AddLaneArray = GetLanePath(NewStation, NextStation);

	int32 lastIndex = AddLaneArray.Num() - 1;
	AddLaneArray.RemoveAt(lastIndex);

	FLanePoint Start = LaneArray[0];
	
	
	LaneArray.Insert(AddLaneArray, 0);

	AddLaneArray.Add(Start);
//Add LaneLoc
//	TArray<FVector> NewLaneLocation;

	if (!GridManagerRef) {
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is not valid."));
		return;
	}

//Set Spline Again
//Add Spline Mesh
	UpdateLocationAndSpline();

	int32 NewPointNum = AddLaneArray.Num();
	SetMeshByIndex(0, NewPointNum - 1);	

	TArray<TArray<FIntPoint>> DeletedBridge = GetArea(AddLaneArray, GridType::Water);
	ConnectBT(DeletedBridge, GridType::Water);

	TArray<TArray<FIntPoint>> DeletedTunnel = GetArea(AddLaneArray, GridType::Hill);
	ConnectBT(DeletedTunnel, GridType::Hill);
	return;

}

void ALane::ExtendEnd(AStation* NewStation){
	//Destroy used-to-be last mesh
	ClearSplineMeshAt(LaneArray.Num() -1);

	//Add Lane Array
	TArray<FLanePoint> AddLaneArray;

	AStation* StartStation = StationPoint.Last(1);

	AddLaneArray = GetLanePath(StartStation, NewStation);

	int32 legacyNum = LaneArray.Num() -1;

	FLanePoint last = LaneArray.Last();

	LaneArray.RemoveAt(LaneArray.Num()-1);

	LaneArray.Insert(AddLaneArray, LaneArray.Num());
	AddLaneArray.Insert(last, 0);

	//Add LaneLoc
	if (!GridManagerRef) {
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is not valid."));
		return;
	}
	UpdateLocationAndSpline();

	int32 NumSplinePoints = LaneSpline->GetNumberOfSplinePoints();

	SetMeshByIndex(legacyNum, LaneArray.Num() - 1);

	TArray<TArray<FIntPoint>> DeletedBridge = GetArea(AddLaneArray, GridType::Water);
	ConnectBT(DeletedBridge, GridType::Water);

	TArray<TArray<FIntPoint>> DeletedTunnel = GetArea(AddLaneArray, GridType::Hill);
	ConnectBT(DeletedTunnel, GridType::Hill);

	return;

}

void ALane::ChangeLaneAppearance(AStation* StartStation, AStation* EndStation)
{
	int CurrentBridgeNum = 0;
	int CurrentTunnelNum = 0;

	ClearSplineMeshAt(0);

	//Get LanePoints
	TArray<FLanePoint> AddLaneArray;

	AddLaneArray = GetLanePath(EndStation, StartStation);
	Algo::Reverse(AddLaneArray);

	//Check Changable with BT
	TArray<TArray<FIntPoint>> TunnelArea = GetConnectorArea(AddLaneArray, GridType::Hill);
	TArray<TArray<FIntPoint>> BridgeArea = GetConnectorArea(AddLaneArray, GridType::Water);

	int32 RequiredTunnel = GetRequiredConnector(TunnelArea, GridType::Hill);
	int32 RequiredBridge = GetRequiredConnector(TunnelArea, GridType::Water);

	bool IsChangable = true;
	if (RequiredBridge > CurrentBridgeNum + TinyMetroPlayerState->GetValidBridgeCount())
	{
		IsChangable = false;
	}
	if (RequiredTunnel > CurrentTunnelNum + TinyMetroPlayerState->GetValidTunnelCount())
	{
		IsChangable = false;
	}

	if (!IsChangable) return;

	//Start
	int32 lastIndex = AddLaneArray.Num() - 1;
	AddLaneArray.RemoveAt(lastIndex); // why?

	FLanePoint Start = LaneArray[0];

	LaneArray.Insert(AddLaneArray, 0);

	AddLaneArray.Add(Start);
	//End


	//Add LaneLoc
	//	TArray<FVector> NewLaneLocation;

	if (!GridManagerRef) {
		UE_LOG(LogTemp, Warning, TEXT("GridManagerRef is not valid."));
		return;
	}

	//Set Spline Again
	//Add Spline Mesh
	UpdateLocationAndSpline();

	int32 NewPointNum = AddLaneArray.Num();
	SetMeshByIndex(0, NewPointNum - 1);

	//why DeletedBridge
	TArray<TArray<FIntPoint>> DeletedBridge = GetArea(AddLaneArray, GridType::Water);
	ConnectBT(DeletedBridge, GridType::Water);

	TArray<TArray<FIntPoint>> DeletedTunnel = GetArea(AddLaneArray, GridType::Hill);
	ConnectBT(DeletedTunnel, GridType::Hill);

}

bool ALane::IsStationsValid(const TArray<class AStation*>& NewStationPoint) {
	if (NewStationPoint.Num() < 2) {
		UE_LOG(LogTemp, Warning, TEXT("Invalid NewStationPoint. The array should contain at least 2 elements."));
		return false;
	}
	for (AStation* Station : NewStationPoint) {
		if (Station == nullptr) {
			UE_LOG(LogTemp, Warning, TEXT("Invalid NewStationPoint. The array contains an invalid Station."));
			return false;
		}
	}

	return true;
}

TArray<FLanePoint> ALane::GetLanePath(AStation* StartStation, AStation* EndStation) {
	//Get Path of Coord
	TArray<FLanePoint> LaneBlock;
	
	FIntPoint StartStationCoord = StartStation->GetCurrentGridCellData().WorldCoordination;
	FIntPoint NextStationCoord = EndStation->GetCurrentGridCellData().WorldCoordination;

	FIntPoint Diff = NextStationCoord - StartStationCoord;

	bool IsBending = HasBendingPoint(Diff);


	if (IsBending)
	{
		FIntPoint BendingCoord = FindBendingPoint(StartStationCoord, NextStationCoord);
		int32 Position = LaneManagerRef->GetPosition(StartStationCoord, BendingCoord);

		AddLanePoint(StartStationCoord, true, true, LaneBlock, Position);

		// Generate paths
		TArray<FIntPoint> PathToBending = GeneratePath(StartStationCoord, BendingCoord);
		TArray<FIntPoint> PathFromBending = GeneratePath(BendingCoord, NextStationCoord);

		// Add PathToBending points to LaneArray
		for (const FIntPoint& Point : PathToBending)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		// Add BendingPoint to LaneArray
		Position = LaneManagerRef->GetPosition(BendingCoord, NextStationCoord);
		AddLanePoint(BendingCoord, false, true, LaneBlock, Position);

		// Add PathFromBending points to LaneArray
		for (const FIntPoint& Point : PathFromBending)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}
		
		AddLanePoint(NextStationCoord, true, true, LaneBlock);

	}
	else
	{
		int32 Position = LaneManagerRef->GetPosition(StartStationCoord, NextStationCoord);

		AddLanePoint(StartStationCoord, true, true, LaneBlock, Position);

		TArray<FIntPoint> PathToNext = GeneratePath(StartStationCoord, NextStationCoord);

		// Add PathToNext points to LaneArray
		for (const FIntPoint& Point : PathToNext)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		AddLanePoint(NextStationCoord, true, true, LaneBlock);

	}


	return LaneBlock;
}

TArray<FLanePoint> ALane::GetLanePathByPoint(FIntPoint StartStation, FIntPoint EndStation) {
	//Get Path of Coord
	TArray<FLanePoint> LaneBlock;

	FIntPoint StartStationCoord = StartStation;
	FIntPoint NextStationCoord = EndStation;

	FIntPoint Diff = NextStationCoord - StartStationCoord;

	bool IsBending = HasBendingPoint(Diff);


	if (IsBending)
	{
		FIntPoint BendingCoord = FindBendingPoint(StartStationCoord, NextStationCoord);
		int32 Position = LaneManagerRef->GetPosition(StartStationCoord, BendingCoord);

		AddLanePoint(StartStationCoord, true, true, LaneBlock, Position);

		// Generate paths
		TArray<FIntPoint> PathToBending = GeneratePath(StartStationCoord, BendingCoord);
		TArray<FIntPoint> PathFromBending = GeneratePath(BendingCoord, NextStationCoord);

		// Add PathToBending points to LaneArray
		for (const FIntPoint& Point : PathToBending)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		// Add BendingPoint to LaneArray
		Position = LaneManagerRef->GetPosition(BendingCoord, NextStationCoord);
		AddLanePoint(BendingCoord, false, true, LaneBlock, Position);

		// Add PathFromBending points to LaneArray
		for (const FIntPoint& Point : PathFromBending)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		AddLanePoint(NextStationCoord, true, true, LaneBlock);

	}
	else
	{
		int32 Position = LaneManagerRef->GetPosition(StartStationCoord, NextStationCoord);

		AddLanePoint(StartStationCoord, true, true, LaneBlock, Position);

		TArray<FIntPoint> PathToNext = GeneratePath(StartStationCoord, NextStationCoord);

		// Add PathToNext points to LaneArray
		for (const FIntPoint& Point : PathToNext)
		{
			AddLanePoint(Point, false, false, LaneBlock, Position);
		}

		AddLanePoint(NextStationCoord, true, true, LaneBlock);

	}


	return LaneBlock;
}

void ALane::SetMeshByIndex(int32 StartIndex, int32 LastIndex){
	if (!LaneSpline) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid input parameters for R2SplineMeshComponent."));
		return;
	}

	// Destroy Exisiting One
	for (int32 i = StartIndex; i <= LastIndex; i++)
	{
		if (!LaneArray.IsValidIndex(i))
		{
			UE_LOG(LogTemp, Warning, TEXT("SetMeshByIndex : Invalid LaneArray Index"));			
		}
		else 
		{
			ClearSplineMeshAt(i);
		}
	}

	FVector StartPos;
	FVector StartTangent;
	FVector EndPos;
	FVector EndTangent;

	float Length;
	float ClampedLength;

	for (int32 i = StartIndex; i <= LastIndex; i++) 
	{
		int32 indexNum = LaneArray[i].SplineIndex.Num();
		for (int32 k = 0; k < indexNum; k++)
		{
			int32 index = LaneArray[i].SplineIndex[k];
			if (index == 0)
			{
				/* --- Front Mesh Only --- */

				// Set Start/End Pos/Tangent
				StartPos = LaneSpline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
				EndPos = ((StartPos + LaneSpline->GetLocationAtSplinePoint(index + 1, ESplineCoordinateSpace::Local)) / 2.0f);


				StartTangent = LaneSpline->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
				Length = StartTangent.Size();
				ClampedLength = FMath::Clamp(Length, 0.0f, SectionLength);
				StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
				EndTangent = StartTangent;

				//Add&Set Spline Mesh Component (mesh)
				SetSplineMeshComponent(StartPos, StartTangent, EndPos, EndTangent, i, 1);
			}
			else if (index == LaneLocation.Num() - 1)
			{
				/* --- Back Mesh Only --- */

				// Set Start/End Pos/Tangent
				StartPos = (LaneSpline->GetLocationAtSplinePoint(index - 1, ESplineCoordinateSpace::Local) + LaneSpline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local)) / 2.0f;
				EndPos = LaneSpline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);

				StartTangent = EndTangent;
				//Add&Set Spline Mesh Component (mesh)
				SetSplineMeshComponent(StartPos, StartTangent, EndPos, EndTangent, i, 3);
			}
			else
			{
				/* --- Back Mesh --- */

// Set Start/End Pos/Tangent
				StartPos = (LaneSpline->GetLocationAtSplinePoint(index - 1, ESplineCoordinateSpace::Local) + LaneSpline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local)) / 2.0f;
				EndPos = LaneSpline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);

				StartTangent = LaneSpline->GetTangentAtSplinePoint(index - 1, ESplineCoordinateSpace::Local);
				Length = StartTangent.Size();
				ClampedLength = FMath::Clamp(Length, 0.0f, SectionLength);
				StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
				EndTangent = StartTangent;

				StartTangent = EndTangent;
				//Set Spline Mesh Component (mesh)
				SetSplineMeshComponent(StartPos, StartTangent, EndPos, EndTangent, i, 3);

				/* --- Middle Mesh (if there is any) --- */
				if (LaneArray[i].IsBendingPoint) {
					// Set Start/End Pos/Tangent
					StartPos = EndPos;
					EndTangent = LaneSpline->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);

					//Set Spline Mesh Component (mesh)
					SetSplineMeshComponent(StartPos, StartTangent, EndPos, EndTangent, i, 2);
				}

				/* --- Front Mesh --- */

				// Set Start/End Pos/Tangent
				StartPos = LaneSpline->GetLocationAtSplinePoint(index, ESplineCoordinateSpace::Local);
				EndPos = ((StartPos + LaneSpline->GetLocationAtSplinePoint(index + 1, ESplineCoordinateSpace::Local)) / 2.0f);

				StartTangent = EndTangent;
				if (LaneArray[i].IsBendingPoint) {
					StartTangent = LaneSpline->GetTangentAtSplinePoint(index, ESplineCoordinateSpace::Local);
					Length = EndTangent.Size();
					ClampedLength = FMath::Clamp(Length, 0.0f, SectionLength);
					StartTangent = StartTangent.GetSafeNormal() * ClampedLength;
				}
				//Set Spline Mesh Component (mesh)
				SetSplineMeshComponent(StartPos, StartTangent, EndPos, EndTangent, i, 1);

			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("SetMeshByIndex."));

}

void ALane::SetSplineMeshComponent(FVector StartPos, FVector StartTangent, FVector EndPos, FVector EndTangent, int32 Index, int32 Pos) 
{
	USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);
	if (LaneArray[Index].IsThrough)
	{
		SetSplineMeshComponent(SplineMeshComponent, ThroughMesh, Index, Pos);
	}
	else
	{
		SetSplineMeshComponent(SplineMeshComponent, LaneMesh, Index, Pos);
	}
	//RemoveMeshMaterial
	if(LaneArray[Index].WillBeRemoved)
	{
		SplineMeshComponent->SetMaterial(0, RemoveMeshMaterial);
	}
	else
	{
		SplineMeshComponent->SetMaterial(0, MeshMaterial);
	}
	SplineMeshComponent->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent, true);
	SplineMeshComponent->AttachToComponent(LaneSpline, FAttachmentTransformRules::KeepWorldTransform);
	SplineMeshComponent->RegisterComponent();
	LaneArray[Index].MeshArray.Add(SplineMeshComponent);
}

void ALane::SpawnLaneMesh()
{
	//Get LaneLocation ?? is this deletable?
	//Set Spline 
	UpdateLocationAndSpline();

	//Set Mesh
	SetMeshByIndex(0, LaneArray.Num() - 1);
}

void ALane::InitLaneSpline()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create the spline component and attach it to the RootComponent
	LaneSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = LaneSpline;
}

void ALane::SetHasSaveFile(bool hasSave)
{
	HasSaveFile = hasSave;
}

void ALane::SubTotalLaneCount()
{
	StatisticsManagerRef->LaneStatistics.TotalLaneCount--;
}

void ALane::AddTotalModifyAndDeleteCount()
{
	StatisticsManagerRef->LaneStatistics.TotalModifyAndDeleteCount++;
}

void ALane::AddModifyAndReduceCountInEachLane()
{
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].TotalModifyAndReduceCount++;
}

void ALane::AddServiceStationCount(int32 Num)
{
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].ServiceStationCount += Num;
}

void ALane::SubServiceStationCount(int32 Num)
{
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].ServiceStationCount -= Num;
}

float ALane::GetStationComplainAverage()
{
	float Sum = 0.0f;

	for (int i = 0; i < StationPoint.Num(); i++)
	{
		if (i == StationPoint.Num() - 1)
		{
			if (!IsCircularLine)
			{
				Sum += StationPoint[i]->GetStationInfo().Complain;

				Sum /= StationPoint.Num();
				break;
			}

			Sum /= StationPoint.Num() - 1;

		}
		else
		{
			Sum += StationPoint[i]->GetStationInfo().Complain;
		}
	}

	return Sum;

}


void ALane::InitializeCurrentLaneStatics()
{
	SubTotalLaneCount();

	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].ServiceStationCount = 0;
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].ServiceTrainAndSubtrainCount = 0;
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].AverageComplain = 0;
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].IsCircularLane = false;
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].TransferStationCount = 0;

	//Bridge & Tunnel
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].UsingBridgeCount = 0;
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].UsingTunnelCount = 0;
}
void ALane::UpdateUsingConnector()
{
	UsingTunnelCount = BTMangerREF->GetUsingConnectorCount(LaneId, ConnectorType::Tunnel);
	UsingBridgeCount = BTMangerREF->GetUsingConnectorCount(LaneId, ConnectorType::Bridge);

	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].UsingBridgeCount = UsingBridgeCount;
	StatisticsManagerRef->LaneStatistics.Lanes[LaneId].UsingTunnelCount = UsingTunnelCount;
}