// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../GridGenerator/GridManager.h"
#include "../GameModes/TinyMetroGameModeBase.h"
#include "../SaveSystem/TMSaveManager.h"
#include "Station.h"
#include "../Policy/Policy.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "AdjList.h"
#include "StationManager.generated.h"

UCLASS()
class TINYMETRO_API AStationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStationManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	friend class ATMSaveManager;
	friend class APolicy;

	UFUNCTION(BlueprintCallable)
	AStation* GetNearestStation(FVector CurrentLocation, class ALane* LaneRef);

protected:
	UFUNCTION(BlueprintCallable)
	void SpawnStation(FGridCellData GridCellData, StationType Type, bool ActivateFlag);
	UFUNCTION(BlueprintCallable)
	StationType GetRandomStationType();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void StationSpawnRoutine();
	void TestFunction();

public:	
	StationType CalculatePassengerDest(StationType Except) const;
	float GetComplainAverage();
	//Policy maintenance cost routine
	void PolicyMaintenanceRoutine();

	UFUNCTION(BlueprintCallable)
	void AddNewStationInAdjList(int32 Id, StationType Type);

	UFUNCTION(BlueprintCallable)
	void AddAdjListItem(AStation* Start, AStation* End, float Length);

	UFUNCTION(BlueprintCallable)
	void RemoveAdjListItem(FIntPoint First, FIntPoint Second);

	UFUNCTION(BlueprintCallable)
	AStation* GetStationByGridCellData(FIntPoint _IntPoint);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	int32 MaxStationCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 StationSpawnRequire = 15000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 StationSpawnPerSec = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	int32 StationSpawnCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station")
	TArray<class AStation*> Station;
	UPROPERTY(BlueprintReadOnly, Category = "Station")
	FTimerHandle TimerSpawnStation;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	AGridManager* GridManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	ATinyMetroGameModeBase* GameMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	APolicy* Policy;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	TMap<FIntPoint, StationType> InitData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	TArray<StationType> StationSpawnTable = {
		StationType::Circle,
		StationType::Circle,
		StationType::Circle,
		StationType::Circle,
		StationType::Triangle,
		StationType::Triangle,
		StationType::Rectangle,
		// Below after 2 weeks
		StationType::Circle,
		StationType::Circle,
		StationType::Circle,
		StationType::Circle,
		StationType::Triangle,
		StationType::Triangle,
		StationType::Rectangle,
		StationType::Cross,
		StationType::Rhombus,
		StationType::Oval,
		StationType::Diamond,
		StationType::Pentagon,
		StationType::Star,
		StationType::Fan
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 StationSpawnRange = 7;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 StationId = 0;

	//Policy Timer
	UPROPERTY(BlueprintReadOnly)
	FTimerHandle PolicyTimerStation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PolicyCostRequire = 10000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PolicyCostPerSec = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 PolicyCostCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATinyMetroPlayerState* PlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAdjList* AdjList;
};
