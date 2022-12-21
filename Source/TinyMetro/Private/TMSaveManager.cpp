// Fill out your copyright notice in the Description page of Project Settings.

#include "TMSaveManager.h"
#include "Kismet/GameplayStatics.h"


TMSaveManager::TMSaveManager()
{
}

TMSaveManager::~TMSaveManager()
{
}

void TMSaveManager::SaveStationManager(AStationManager* stationmanager) {

	UStationSaveGame* StationSaveData;

	if (UGameplayStatics::DoesSaveGameExist("StationSave", 0)) {
		StationSaveData = Cast<UStationSaveGame>(UGameplayStatics::LoadGameFromSlot("StationSave", 0));
	}
	else {
		StationSaveData = Cast<UStationSaveGame>(UGameplayStatics::CreateSaveGameObject(UStationSaveGame::StaticClass()));
	}

	if (StationSaveData == NULL) {
		return;
	}

	StationSaveData->StationSpawnCurrent = stationmanager->StationSpawnCurrent;
	StationSaveData->ManagerStationId = stationmanager->StationId;

	int32 size = stationmanager->Station.Num();

	for (int i = 0; i < size; i++)
	{
		StationSaveData->stations.Empty();

		AStation* temp = stationmanager->Station[i];

		FStationValuesStruct stationValue;

		stationValue.ComplainCurrent = temp->ComplainCurrent;
		stationValue.StationId = temp->StationId;
		stationValue.IsActive = temp->IsActive;
		stationValue.StationTypeValue = temp->StationTypeValue;
		stationValue.GridCellData = temp->GridCellData;
		//complain, stationid, isActive, stationtypevalue

		StationSaveData->stations.Add(stationValue);
	}

	if (!UGameplayStatics::SaveGameToSlot(StationSaveData, "StationSave", 0))
	{
		UE_LOG(LogTemp, Error, TEXT("StationSaveGame Error!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("StationSaveGame Success!"));
	}
}

void TMSaveManager::LoadStationManager(AStationManager* stationmanager) {

	if (UGameplayStatics::DoesSaveGameExist("StationSave", 0))
	{
		UStationSaveGame* StationLoadData = Cast<UStationSaveGame>(UGameplayStatics::LoadGameFromSlot("StationSave", 0));

		// 액터 배치?

		stationmanager->StationSpawnCurrent = StationLoadData->StationSpawnCurrent;
		stationmanager->StationId = StationLoadData->ManagerStationId;

		stationmanager->Station.Empty();

		int32 size = StationLoadData->stations.Num();

		for (int i = 0; i < size; i++)
		{
			FStationValuesStruct stationValue = StationLoadData->stations[i];

			SpawnStations(stationmanager, stationValue.GridCellData, stationValue.StationTypeValue, stationValue.StationId, stationValue.ComplainCurrent, stationValue.IsActive);
		}
	}

}

void TMSaveManager::SpawnStations(AStationManager* stationmanager, FGridCellData GridCellData, StationType Type, int32 StationId, int32 ComplainCurrent, bool ActivateFlag) {
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Station/BP_Station.BP_Station'")));

	// Cast to BP
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);

	// Check object validation
	if (!SpawnActor) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}

	// Check null
	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return;
	}

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = stationmanager;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AStation* tmp = Cast<AStation>(stationmanager->GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, GridCellData.WorldLocation, stationmanager->GetActorRotation(), SpawnParams));
	tmp->SetStationType(Type);
	tmp->SetStationId(StationId);
	tmp->SetGridCellData(GridCellData);
	tmp->SetComplainCurrent(ComplainCurrent);
	if (ActivateFlag) {
		tmp->ActivateStation();
	}

	stationmanager->Station.Add(tmp);
	stationmanager->GridManager->SetGridStructure(
		GridCellData.WorldCoordination.X,
		GridCellData.WorldCoordination.Y,
		GridStructure::Station);
}


void TMSaveManager::SaveGetWorld(UWorld* tmworld) {
	
	UWorldSaveGame* WorldSaveData;

	if (UGameplayStatics::DoesSaveGameExist("WorldSave", 0)) {
		WorldSaveData = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot("WorldSave", 0));
	}
	else {
		WorldSaveData = Cast<UWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UWorldSaveGame::StaticClass()));
	}

	if (WorldSaveData == NULL) {
		return;
	}

	WorldSaveData->TMWorld = tmworld;

	if (!UGameplayStatics::SaveGameToSlot(WorldSaveData, "WorldSave", 0))
	{
		UE_LOG(LogTemp, Error, TEXT("StationSaveGame Error!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("StationSaveGame Success!"));
	}

}

void TMSaveManager::LoadGetWorld(UWorld* tmworld) {
	
	if (UGameplayStatics::DoesSaveGameExist("WorldSave", 0))
	{
		UWorldSaveGame* WorldLoadData = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot("WorldSave", 0));

		// 액터 배치?

		tmworld = WorldLoadData->TMWorld;
	}
}