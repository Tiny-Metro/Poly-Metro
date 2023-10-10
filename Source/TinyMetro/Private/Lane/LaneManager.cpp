// Fill out your copyright notice in the Description page of Project Settings.

#include "Lane/LaneManager.h"
#include "Misc/OutputDeviceNull.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <Engine/AssetManager.h>
#include <Kismet/GameplayStatics.h>
#include "Lane/LaneManagerSaveGame.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManager.h"
#include "Station/Station.h"


// Sets default values
ALaneManager::ALaneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LaneClass = ConstructorHelpers::FObjectFinder<UClass>(TEXT("Class'/Game/Lane/BP_Lane.BP_Lane_C'")).Object;
}

// Called when the game starts or when spawned
void ALaneManager::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	StationManagerRef = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));
	SaveManagerRef = GameMode->GetSaveManager();
	StatisticsManagerRef = GameMode->GetStatisticsManager();
	
	InitLaneMaterial();

	SaveManagerRef->SaveTask.AddDynamic(this, &ALaneManager::Save);
}

// Called every frame
void ALaneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ALaneManager::GetLaneCount() const {
	int32 result = 0;
	for (auto& i : Lanes) {
		if (IsValid(i.Value)) {
			result++;
		}
	}
	return result;
}

int32 ALaneManager::GetRandomLaneId() const {
	int32 randId = FMath::RandRange(0, Lanes.Num() - 1);
	do {
		randId = FMath::RandRange(0, Lanes.Num() - 1);
	} while (!IsValid(Lanes[randId]));
	return randId;
}


void ALaneManager::SetCanAssginBridge(bool AssginBridge)
{
	CanAssignBridge = AssginBridge;
}

void ALaneManager::SetCanAssginTunnel(bool AssginTunnel)
{
	CanAssignTunnel = AssginTunnel;
}

void ALaneManager::AddNextLaneNums(int32 LaneNum)
{
	NextLaneNums.Add(LaneNum);

	NextLaneNums.Sort();
}

void ALaneManager::RemoveNextLaneNums()
{
	NextLaneNums.RemoveAt(0);
}

void ALaneManager::RemoveDestroyedLane(int LaneNum)
{
	Lanes.Remove(LaneNum);
}

void ALaneManager::SetUILaneNum(int32 Num)
{
	UILaneNum = Num;

	if (UILaneNum == 0)
	{
		LaneCancelDelegate.Broadcast();
	}
	else
	{
		LaneSelectDelegate.Broadcast();
	}
}

int32 ALaneManager::GetUILaneNum()
{
	return UILaneNum;
}

void ALaneManager::SetSelectedLaneNum(int32 Num)
{
	SelectedLaneNum = Num;

	SetUILaneNum(Num);
}

void ALaneManager::AddSelectedStations(AStation* Station)
{
	if (SelectedStations.Num() == 1 && SelectedStations[0] ==Station)
	{
		return;
	}

	SelectedStations.Add(Station);

	if (IsPlacementValid)
	{
		//ExtendLane
		Lanes[SelectedLaneNum]->ExtendLane(Station);
		SelectedStations.Empty();
	}
	else
	{

		if (SelectedStations.Num() == 2)
		{
			//Add NewLane
			CreatingNewLane();

			CancelSelectedStations();
		}
		else
		{
			SetUILaneNum(NextLaneNums[0]);
		}

	}
}

void ALaneManager::CancelSelectedStations()
{
	SelectedStations.Empty();

	SetUILaneNum(0);

	IsPlacementValid = false;
}

ALane* ALaneManager::SpawnLane()
{
	// Load BP Class
	//UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Lane/BP_Lane.BP_Lane'")));
	if (!LaneClass) {
		LaneClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Class'/Game/Lane/BP_Lane.BP_Lane_C'")));
	}

	// Cast to BP
	//UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	//// Check object validation
	//if (!SpawnActor) {
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN / Lane")));
	//	return nullptr;
	//}

	// Check null
	/*UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return nullptr;
	}*/

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	ALane* tmpLane = Cast<ALane>(GetWorld()->SpawnActor<AActor>(LaneClass, SpawnParams));
	
	return tmpLane;
}

void ALaneManager::CreatingNewLane() {

	if (!(PlayerStateRef->UseLane())) {

		UE_LOG(LogTemp, Warning, TEXT("Already used up the lane, so I can't make a new one. "));
		return;
	}

	ALane* tmpLane = SpawnLane();

	//ALane* tmpLane = GetWorld()->SpawnActor<>();
	tmpLane->SetLaneId(NextLaneNums[0]);

	UE_LOG(LogTemp, Warning, TEXT("GetActorNameOrLabel : %s"), *(tmpLane->GetActorNameOrLabel()));

	

	UE_LOG(LogTemp, Warning, TEXT("SelectedStations[%d]"), SelectedStations.Num());
	for (int i = 0; i < SelectedStations.Num(); i++) {
		
		if (IsValid(SelectedStations[i])) {
			SelectedStations[i]->SetLanes(NextLaneNums[0]);
			SelectedStations[i]->SetActivate(true);
			//FIntPoint Coor = SelectedStations[i]->GetCurrentGridCellData().WorldCoordination;
			tmpLane->StationPoint.Add(SelectedStations[i]);

		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("SelectedStations[%d] is null"), i);
			return;
		}
		
	}

	/*
	FIntPoint Start = SelectedStations[0]->GetCurrentGridCellData().WorldCoordination;
	FIntPoint End = SelectedStations[1]->GetCurrentGridCellData().WorldCoordination;
	UE_LOG(LogTemp, Warning, TEXT("CreatingNewLane /IntPoint Start : %d, %d"), Start.X, Start.Y);
	UE_LOG(LogTemp, Warning, TEXT("CreatingNewLane /IntPoint End : %d, %d"), End.X, End.Y);
	*/

	tmpLane->AddAdjListDistance(SelectedStations[0], SelectedStations[1]);

	tmpLane->InitLaneMaterial(LaneMaterial);
	tmpLane->InitializeNewLane();

	Lanes.Add(NextLaneNums[0], tmpLane);
	//Lanes.Add(tmpLane);

	StatisticsManagerRef->LaneStatistics.TotalLaneCount++;
	StatisticsManagerRef->LaneStatistics.Lanes[tmpLane->GetLaneId()].ServiceStationCount += 2;
	
	CheckTransferStation();

	tmpLane->SpawnTrain();

	UE_LOG(LogTemp, Warning, TEXT("StationPoint Num : %d"), tmpLane->StationPoint.Num());

	UE_LOG(LogTemp, Warning, TEXT("New LaneNum : %d"), NextLaneNums[0]);

	RemoveNextLaneNums();

	//StationManagerRef->AddAdjListItem(SelectedStations[0], SelectedStations[1], )
}


//void ALaneManager::AddStationInLane(int CurrentLane) {
//
//	ALane* tmp = GetLaneById(CurrentLane);
//
//	tmp->ExtendLane();
//}

void ALaneManager::AddLane(ALane* Obj) {

}

ALane* ALaneManager::GetLaneById(int32 LaneId) {

	ALane* Lane = Lanes.FindRef(LaneId);
	return Lane;
}

void ALaneManager::InitLaneMaterial() {
	LaneMaterialPath = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetLaneMaterialPath();

	auto& AssetLoader = UAssetManager::GetStreamableManager();
	/*
	AssetLoader.RequestAsyncLoad(
		LaneMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ALaneManager::LaneMaterialDeferred)
	);
	AssetLoader.RequestAsyncLoad(
		LaneMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ALaneManager::LaneMaterialDeferred)
	);
	*/
	for (auto& Path : LaneMaterialPath)
	{
		// Load Material Synchronously
		UMaterial* MaterialAsset = Cast<UMaterial>(StaticLoadObject(UMaterial::StaticClass(), nullptr, *Path.ToString()));

		if (MaterialAsset)
		{
			LaneMaterial.Add(MaterialAsset);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load material at path: %s"), *Path.ToString());
		}
	}

}

void ALaneManager::LaneMaterialDeferred() {

	for (auto& i : LaneMaterialPath) {
		//TAssetPtr<UMaterial> tmp(i);
		LaneMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

TArray<UMaterial*> ALaneManager::GetLaneMaterial() const {
	return LaneMaterial;
}

//<<<<<<< HEAD
int32 ALaneManager::GetPosition(FIntPoint Start, FIntPoint End) {
	
	UE_LOG(LogTemp, Warning, TEXT("Start Getting Position"));

	TArray<int32> TakenPosition;
	FIntPoint Diff = Start - End;

	for (const auto& Pair : Lanes)
	{
		const ALane* TargetLane = Pair.Value;
		if (!IsValid(TargetLane))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid Lane detected, skipping process"));
			continue;
		}
		int32 LaneId = TargetLane->GetLaneId();
		UE_LOG(LogTemp, Warning, TEXT("GetPosition: Checking Lane ID = %d"), LaneId);
		TArray<FLanePoint> TargetLaneArray = TargetLane->LaneArray;
		if (!TargetLaneArray.IsValidIndex(0))
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid LaneArray detected, skipping process"));
			continue;
		}


		FIntPoint TargetStart = TargetLaneArray[0].Coordination;
		FIntPoint TargetEnd;
		// GetBending Cord
		for (int32 k = 1; k < TargetLaneArray.Num(); k++) {
			if (TargetLaneArray[k].IsBendingPoint == true) {
				TargetEnd = TargetLaneArray[k].Coordination;
				

				FIntPoint TargetDiff = TargetStart - TargetEnd;
				//Check if same Direction
				if ((Diff.X == 0 && TargetDiff.X == 0)) {
					if (Start.X == TargetStart.X) {
						UE_LOG(LogTemp, Warning, TEXT("GetPosition: X!!! %d = X %d position Taken %d Lane ID = %d"), Start.X, TargetStart.X, TargetLaneArray[k - 1].LanePosition, LaneId);

						if ((Start.Y > FMath::Min(TargetStart.Y, TargetEnd.Y) && Start.Y < FMath::Max(TargetStart.Y, TargetEnd.Y)) ||
							(End.Y > FMath::Min(TargetStart.Y, TargetEnd.Y) && End.Y < FMath::Max(TargetStart.Y, TargetEnd.Y)) ||
							(FMath::Min(Start.Y, End.Y) <= FMath::Min(TargetStart.Y, TargetEnd.Y) && FMath::Max(Start.Y, End.Y) >= FMath::Max(TargetStart.Y, TargetEnd.Y)))
						{
							TakenPosition.Add(TargetLaneArray[k - 1].LanePosition);
							UE_LOG(LogTemp, Warning, TEXT("GetPosition: position Taken %d Lane ID = %d"), TargetLaneArray[k - 1].LanePosition, LaneId);

						}
					}
					continue;
				}
				
				if (Diff.Y == 0 && TargetDiff.Y == 0) {
					if (Start.Y == TargetStart.Y) {
						UE_LOG(LogTemp, Warning, TEXT("GetPosition: Y!!! %d = Y %d position Taken %d Lane ID = %d"), Start.Y, TargetStart.Y, TargetLaneArray[k - 1].LanePosition, LaneId);

						if ((Start.X > FMath::Min(TargetStart.X, TargetEnd.X) && Start.X < FMath::Max(TargetStart.X, TargetEnd.X)) ||
							(End.X > FMath::Min(TargetStart.X, TargetEnd.X) && End.X < FMath::Max(TargetStart.X, TargetEnd.X)) ||
							(FMath::Min(Start.X, End.X) <= FMath::Min(TargetStart.X, TargetEnd.X) && FMath::Max(Start.X, End.X) >= FMath::Max(TargetStart.X, TargetEnd.X)))
						{
							TakenPosition.Add(TargetLaneArray[k - 1].LanePosition);
							UE_LOG(LogTemp, Warning, TEXT("GetPosition: position Taken %d Lane ID = %d"), TargetLaneArray[k - 1].LanePosition, LaneId);

						}
					}
					continue;

				}

				if(Diff.X * TargetDiff.Y == Diff.Y * TargetDiff.X) {
					UE_LOG(LogTemp, Warning, TEXT("GetPosition: Diff.X * TargetDiff.Y == Diff.Y * TargetDiff.X  Lane ID = %d"), LaneId);

					UE_LOG(LogTemp, Warning, TEXT("GetPosition: Diff target = %d, the diff = %d  Lane ID = %d"), (Start.Y - Start.X), (TargetStart.Y - TargetStart.X), LaneId);
					UE_LOG(LogTemp, Warning, TEXT("GetPosition: targetStart = (%d, %d),  Start = ( %d, %d)  Lane ID = %d"), TargetStart.X, TargetStart.Y, Start.X, Start.Y, LaneId);

					if ( (Start.Y - Start.X) == (TargetStart.Y - TargetStart.X) || (Start.Y + Start.X) == (TargetStart.Y + TargetStart.X))
					{
						UE_LOG(LogTemp, Warning, TEXT("GetPosition:  (Start.Y - Start.X) == TargetStart.Y - TargetStart.X)   Lane ID = %d"), LaneId);

						if ( ( (Start.X > FMath::Min(TargetStart.X, TargetEnd.X) ) && (Start.X < FMath::Max(TargetStart.X, TargetEnd.X) ) ) ||
							( (End.X > FMath::Min(TargetStart.X, TargetEnd.X) ) && ( End.X < FMath::Max(TargetStart.X, TargetEnd.X)) ) ||
							( (FMath::Min(Start.X, End.X) <= FMath::Min(TargetStart.X, TargetEnd.X) ) && (FMath::Max(Start.X, End.X) >= FMath::Max(TargetStart.X, TargetEnd.X) ) ))
						{
							TakenPosition.Add(TargetLaneArray[k - 1].LanePosition);
							UE_LOG(LogTemp, Warning, TEXT("GetPosition: position Taken %d Lane ID = %d"), TargetLaneArray[k - 1].LanePosition, LaneId);

						}
					}
					continue;
				}

				TargetStart = TargetEnd;
			}

		}
	}

	if (TakenPosition.Num() == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Position : 0"));
		return 0;
	}
	for (int32 i = 0; i < TakenPosition.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Taken Position : %d"), TakenPosition[i] );
	}

	for (int32 i = 0; i < 10; i++) {
		if (TakenPosition.Contains(i) == false) {
			UE_LOG(LogTemp, Warning, TEXT("Position : %d"), i);
			return i;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Position : INVALID"));
	return - 1;
}

void ALaneManager::Save()
{
	ULaneManagerSaveGame* tmp = Cast<ULaneManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(ULaneManagerSaveGame::StaticClass()));

	tmp->CanAssignBridge = CanAssignBridge;
	tmp->CanAssignTunnel = CanAssignTunnel;
	tmp->NextLaneNums = NextLaneNums;

	for (auto& i : Lanes)
	{
		tmp->Lanes.Add(i.Key);
	}

	SaveManagerRef->Save(tmp, SaveActorType::LaneManager);
}

bool ALaneManager::Load()
{ 
	if (!GameMode) {
		GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	if (!SaveManagerRef) {
		SaveManagerRef = GameMode->GetSaveManager();
	}

	InitLaneMaterial();

	ULaneManagerSaveGame* tmp = Cast<ULaneManagerSaveGame>(SaveManagerRef->Load(SaveActorType::LaneManager));

	if (!IsValid(tmp)) 
	{
		return false;
	}

	CanAssignBridge = tmp->CanAssignBridge;
	CanAssignTunnel = tmp->CanAssignTunnel;
	NextLaneNums = tmp->NextLaneNums;

	for (auto& i : tmp->Lanes)
	{
		ALane* tmpLane = LoadLane(i);

		Lanes.Add(i, tmpLane);
	}

	for (const auto& i : Lanes)
	{
		i.Value->SetHandleTransform();
	}

	return true;

}

ALane* ALaneManager::LoadLane(int32 LaneId)
{

	ALane* tmpLane = SpawnLane();

	tmpLane->SetLaneId(LaneId);

	if (LaneMaterial.IsValidIndex(7))
	{
		tmpLane->InitLaneMaterial(LaneMaterial);

		bool hasSave = tmpLane -> Load();

		tmpLane->SetHasSaveFile(hasSave);
	}

	return tmpLane;
}

void ALaneManager::CheckTransferStation()
{
	UE_LOG(LogTemp, Warning, TEXT("In?"));

	for (auto& Item : Lanes)
	{
		ALane* Lane = Item.Value;
		int32 Sum = 0;

		for (int i = 0; i<Lane->StationPoint.Num(); i++)
		{
			if (i == Lane->StationPoint.Num() - 1)
			{
				if (Lane->GetIsCircularLine())
				{
					break;
				}
				else
				{
					if (Lane->StationPoint[i]->GetStationInfo().ServiceLaneCount >= 2)
					{
						Sum++;
					}
				}
			}
			else
			{
				if (Lane->StationPoint[i]->GetStationInfo().ServiceLaneCount >= 2)
				{
					Sum++;
				}
			}

			UE_LOG(LogTemp, Warning, TEXT("Lane %d , StationPoint %d , Sum :: %d"), Item.Key, i, Sum);
		}

		StatisticsManagerRef->LaneStatistics.Lanes[Item.Key].TransferStationCount = Sum;
	}
}
