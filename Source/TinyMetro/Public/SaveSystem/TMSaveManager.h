// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "StationSaveGame.h"

#include "WorldSaveGame.h"

#include "GridGenerator/GridCellData.h"
#include "GridGenerator/GridManager.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TMSaveManager.generated.h"

class AStation;
class AStationManager;

UCLASS()
class TINYMETRO_API ATMSaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATMSaveManager();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


public:
	AStationManager* stationmanager;
	UWorld* world;

public:
	void SaveStationManager();
	void LoadStationManager();

	void SpawnStations( FGridCellData GridCellData, StationType Type, int32 StationId, int32 ComplainCurrent, bool ActivateFlag);

	void SaveGetWorld();
	void LoadGetWorld();

};
