// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TrainSpawnData.h"
#include "SubtrainSpawnData.h"
#include "TrainManagerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTrainManagerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	int32 NextTrainId;
	UPROPERTY(BlueprintReadWrite)
	TArray<FTrainSpawnData> TrainSapwnData;
	UPROPERTY(BlueprintReadWrite)
	TArray<FSubtrainSpawnData> SubtrainSapwnData;
	UPROPERTY(BlueprintReadWrite)
	float CostUpgradeTrain;
	UPROPERTY(BlueprintReadWrite)
	float CostUpgradeSubtrain;

};
