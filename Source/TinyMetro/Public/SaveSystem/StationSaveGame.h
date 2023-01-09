// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Station/StationType.h"
#include "GridGenerator/GridCellData.h"
#include "StationSaveGame.generated.h"


USTRUCT(BlueprintType)
struct TINYMETRO_API FPassengerValues
{
	GENERATED_BODY()

public:
	UPROPERTY()
		StationType Destination;
};


USTRUCT(BlueprintType)
struct TINYMETRO_API FStationValuesStruct
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 ComplainCurrent;
	UPROPERTY()
		int32 StationId;
	UPROPERTY()
		bool IsActive;
	UPROPERTY()
		StationType StationTypeValue;
	UPROPERTY()
		FGridCellData GridCellData;
	UPROPERTY()
		TArray <FPassengerValues> passengers;
};



UCLASS()
class TINYMETRO_API UStationSaveGame : public USaveGame
{
	GENERATED_BODY()

public :
	UPROPERTY()
		TArray <FStationValuesStruct> stations;
	
		int32 StationSpawnCurrent;
	UPROPERTY()
		int32 ManagerStationId ; 

};
