// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "StationSpawnData.h"
#include "StationManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UStationManagerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 NextStationId;
	UPROPERTY(BlueprintReadWrite)
	bool IsPassengerSpawnEnable;
	UPROPERTY(BlueprintReadWrite)
	TArray<FStationSpawnData> StationSpawnDataArr;
};
