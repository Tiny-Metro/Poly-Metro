// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Station/Station.h"
#include "Station/StationManager.h"
#include "StationSaveGame.h"

#include "WorldSaveGame.h"

#include "GridGenerator/GridCellData.h"
#include "GridGenerator/GridManager.h"

#include "CoreMinimal.h"

/**
 * 
 */
class TINYMETRO_API TMSaveManager
{
public:
	TMSaveManager();
	~TMSaveManager();

	friend class Station;
	friend class StationManager;


public :

	void SaveStationManager(class AStationManager* stationmanager);
	void LoadStationManager(class AStationManager* stationmanager);

	void SpawnStations(AStationManager* stationmanager, FGridCellData GridCellData, StationType Type, int32 StationId, int32 ComplainCurrent, bool ActivateFlag);


	void SaveGetWorld(UWorld* tmworld);
	void LoadGetWorld(UWorld* tmworld);

};
