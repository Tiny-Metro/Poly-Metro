// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StationSaveGame.h"
#include "WorldSaveGame.h"

#include "GridGenerator/GridCellData.h"
#include "GridGenerator/GridManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TMSaveManager.generated.h"

class AStationManager;
class AStation;
class ATinyMetroPlayerState;


UCLASS()
class TINYMETRO_API ATMSaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATMSaveManager();

	friend class Station;
	friend class StationManager;
	friend class TinyMetroPlayerState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	AStationManager* stationmanager;
	ATinyMetroPlayerState* TinyMetroPlayerState; 

	FTimerHandle TimerAutoSave;

	int32 AutoSaveCount = 1000;
	int32 AutoSaveCurrent = 0;
	int32 AutoSavePeriod = 7;
	int32 AutoSaveRequire = 84000; // 12 * 7 * 1000

	void SaveStationManager();
	void LoadStationManager();

	void SpawnStations(FStationValuesStruct StationValues);


	void DeleteSaveFiles();

	void AutoSave();

	void SaveWorldInfo();
	void LoadWorldInfo();

};
