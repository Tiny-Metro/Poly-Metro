// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Station/StationType.h"
#include "StationSaveGame.generated.h"

struct StationValuesStruct
{
public:
	UPROPERTY()
		int32 ComplainCurrent;
	UPROPERTY()
		int32 StationId;
	UPROPERTY()
		bool IsActive;
	UPROPERTY()
		StationType StationTypeValue;

};



UCLASS()
class TINYMETRO_API UStationSaveGame : public USaveGame
{
	GENERATED_BODY();

public : 
	UStationSaveGame();

	UPROPERTY()
		TArray<StationValuesStruct> stations;
	UPROPERTY()
		int32 StationSpawnCurrent;


};
