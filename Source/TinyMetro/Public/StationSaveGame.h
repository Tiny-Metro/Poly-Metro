// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StationSaveGame.generated.h"

struct StationValuesStruct
{
public:
	int stationvalues;

};



UCLASS()
class TINYMETRO_API UStationSaveGame : public USaveGame
{
	GENERATED_BODY();

public : 
	UStationSaveGame();

	UPROPERTY()
		TArray<StationValuesStruct> stations;
};
