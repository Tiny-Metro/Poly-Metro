// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Station/Station.h"
#include "Station/StationManager.h"
#include "StationSaveGame.h"
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

	void SaveStationManager();
	void LoadStationManager();
};
