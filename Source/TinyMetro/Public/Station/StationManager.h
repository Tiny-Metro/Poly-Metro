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
#include "StationInfo.h"
#include "AdjList.h"
#include "StationManager.generated.h"

USTRUCT(BlueprintType)
struct TINYMETRO_API FAdjArray {
	GENERATED_USTRUCT_BODY()
public:
	float& operator[] (int32 Index) { 
		if (Index > Size.Num()) Size.SetNum(Index);
		return Size[Index]; 
	}
	void Init(const float& Element, int32 Number) { Size.Init(Element, Number); }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<float> Size;
};

USTRUCT(BlueprintType)
struct TINYMETRO_API FPath {

	GENERATED_USTRUCT_BODY()
public:
	int& operator[] (int32 Index) {
		if (Index > Size.Num()) Size.SetNum(Index);
		return Size[Index];
	}
	void Init(const int& Element, int32 Number) { Size.Init(Element, Number); }
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<int> Size;
};

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
	void AddAdjListItem(AStation* Start, AStation* End, float Length);

	UFUNCTION(BlueprintCallable)
	void RemoveAdjListItem(AStation* Start, AStation* End);

	UFUNCTION(BlueprintCallable)
	AStation* GetStationByStationInfo(FStationInfo Info);
	UFUNCTION(BlueprintCallable)
	AStation* GetStationById(int32 Id);

	PathQueue GetShortestPath(int32 Start, StationType Type);

	UFUNCTION(BlueprintCallable)
	void AddPassengerSpawnProbability(float Rate, int32 Period);
	UFUNCTION(BlueprintCallable)
	void AddFreePassengerSpawnProbability(float Rate, int32 Period);
	UFUNCTION(BlueprintCallable)
	void AddComplainIncreaseRate(float Rate, int32 Period);
	UFUNCTION(BlueprintCallable)
	void SetServiceData(FServiceData _ServiceData);

	UFUNCTION(BlueprintCallable)
	void NotifySpawnPassenger(StationType Type, bool IsFree);
	UFUNCTION(BlueprintCallable)
	TMap<StationType, int32> GetSpawnPassengerStatistics(int32& TotalPassenger, int32& WaitPassenger, int32 StationId = -1);
	

	UFUNCTION()
	void WeeklyTask() const;
	UFUNCTION()
	void DailyTask() const;

	UFUNCTION()
	void SetTransfer(bool Flag);

private:
	AStation* GetNearestStationByType(int32 Start, StationType Type);
	void FloydWarshall();
	PathQueue PathFinding(int32 Start, StationType Type);

	void PrintPath(int32 Start, StationType Type, TQueue<int32>* Path);


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
	APolicy* PolicyRef;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Passenger")
	TMap<StationType, int32> TotalSpawnPassengerNotFree = {
		TPair<StationType, int32>(StationType::Circle, 0),
		TPair<StationType, int32>(StationType::Triangle, 0),
		TPair<StationType, int32>(StationType::Rectangle, 0),
		TPair<StationType, int32>(StationType::Cross, 0),
		TPair<StationType, int32>(StationType::Rhombus, 0),
		TPair<StationType, int32>(StationType::Oval, 0),
		TPair<StationType, int32>(StationType::Diamond, 0),
		TPair<StationType, int32>(StationType::Pentagon, 0),
		TPair<StationType, int32>(StationType::Star, 0),
		TPair<StationType, int32>(StationType::Fan, 0)
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Passenger")
	TMap<StationType, int32> TotalSpawnPassengerFree = {
		TPair<StationType, int32>(StationType::Circle, 0),
		TPair<StationType, int32>(StationType::Triangle, 0),
		TPair<StationType, int32>(StationType::Rectangle, 0),
		TPair<StationType, int32>(StationType::Cross, 0),
		TPair<StationType, int32>(StationType::Rhombus, 0),
		TPair<StationType, int32>(StationType::Oval, 0),
		TPair<StationType, int32>(StationType::Diamond, 0),
		TPair<StationType, int32>(StationType::Pentagon, 0),
		TPair<StationType, int32>(StationType::Star, 0),
		TPair<StationType, int32>(StationType::Fan, 0)
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Passenger")
	TMap<StationType, int32> TotalSpawnPassenger = {
		TPair<StationType, int32>(StationType::Circle, 0),
		TPair<StationType, int32>(StationType::Triangle, 0),
		TPair<StationType, int32>(StationType::Rectangle, 0),
		TPair<StationType, int32>(StationType::Cross, 0),
		TPair<StationType, int32>(StationType::Rhombus, 0),
		TPair<StationType, int32>(StationType::Oval, 0),
		TPair<StationType, int32>(StationType::Diamond, 0),
		TPair<StationType, int32>(StationType::Pentagon, 0),
		TPair<StationType, int32>(StationType::Star, 0),
		TPair<StationType, int32>(StationType::Fan, 0)
	};

	//Policy Timer
	/*UPROPERTY(BlueprintReadOnly)
	FTimerHandle PolicyTimerStation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PolicyCostRequire = 10000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PolicyCostPerSec = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PolicyCostCurrent = 0;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATinyMetroPlayerState* PlayerState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FServiceData ServiceData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAdjList* AdjList;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAdjArray> AdjDist;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FPath> AdjPath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FAdjArray> adj;

	TMap<int32, TMap<StationType, TQueue<int32>*>> ShortestRoute;
	
	TMap<int32, TMap<StationType, PathQueue>> ShortestPath;
};
