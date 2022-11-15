// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Station.h"
#include "../GridGenerator/GridManager.h"
#include "../GameModes/TinyMetroGameModeBase.h"
#include "StationManager.generated.h"

UCLASS()
class TINYMETRO_API AStationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestFunction();

	UFUNCTION(BlueprintCallable)
	void SpawnStation(FGridCellData& GridCellData);
	UFUNCTION(BlueprintCallable)
	void IncreaseSpawnParameter();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 StationSpawnRequire = 15000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 StationSpawnPerSec = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 StationSpawnCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station")
	TArray<AStation*> Station;
	UPROPERTY(BlueprintReadOnly, Category = "Station")
	FTimerHandle TimerSpawnStation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	AGridManager* GridManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	ATinyMetroGameModeBase* GameMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	TMap<FIntPoint, StationType> InitData;

};
