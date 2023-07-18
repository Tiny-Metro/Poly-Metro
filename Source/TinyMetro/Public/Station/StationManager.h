// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../GridGenerator/GridManager.h"
#include "../GameModes/TinyMetroGameModeBase.h"
#include "Station.h"
#include "../Policy/Policy.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "StationInfo.h"
#include "StationSpawnData.h"
#include "PathQueue.h"
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

	friend class APolicy;

	UFUNCTION()
	void InitStationInfoWidget();
	UFUNCTION()
	UStationInfoWidget* GetStationInfoWidget() const;
	UFUNCTION()
	void InitStationSpawnWidget();

	UFUNCTION(BlueprintCallable)
	AStation* GetNearestStation(FVector CurrentLocation, class ALane* LaneRef);
	UFUNCTION(BlueprintCallable)
	void SpawnStation(FGridCellData GridCellData, StationType Type, int32 Id = -1);
	UFUNCTION(BlueprintCallable)
	StationType GetRandomStationType(); 
	UFUNCTION(BlueprintCallable)
	static StationType StationTypeFromString(FString Str, bool& Success);
	UFUNCTION(BlueprintCallable)
	static FString StationTypeToString(StationType Type, bool& Success);
	
	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	bool Load();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void StationSpawnRoutine(float DeltaTime);
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
	UFUNCTION(BlueprintCallable)
	TArray<AStation*> GetAllStations();

	PathQueue GetShortestPath(int32 Start, StationType Type);

	// Passenger Spawn
	UFUNCTION()
	void PassengerPropertyTick(float DeltaTime);
	UFUNCTION()
	float GetDefaultPassengerSpawnSpeed() const;
	UFUNCTION()
	float GetDefaultPassengerSpawnProbability() const;
	UFUNCTION()
	float GetDefaultFreePassengerSpawnProbability() const;
	UFUNCTION()
	float GetPassengerSpawnSpeed(StationType Type) const;
	/*UFUNCTION()
	float GetPassengerSpawnProbability(StationType Type) const;
	UFUNCTION()
	float GetFreePassengerSpawnProbability(StationType Type) const;
	UFUNCTION()
	void AddPassengerSpawnSpeed(float Rate, StationType Typpe, int32 Day = -1);
	UFUNCTION()
	void MultiplePassengerSpawnSpedd(float Rate, StationType Type, int32 Day = -1);
	UFUNCTION()
	void SetPassengerSpawnSpeed(float Rate, StationType Type);
	UFUNCTION()
	void AddPassengerSpawnProbability(float Rate, StationType Type, int32 Day = -1);
	UFUNCTION()
	void MultiplePassengerSpawnProbability(float Rate, StationType Type, int32 Day = -1);
	UFUNCTION()
	void SetPassengerSpawnProbability(float Rate, StationType Type);
	UFUNCTION()
	void AddFreePassengerSpawnProbability(float Rate, StationType Type, int32 Day = -1);
	UFUNCTION()
	void MultipleFreePassengerSpawnProbability(float Rate, StationType Type, int32 Day = -1);
	UFUNCTION()
	void SetFreePassengerSpawnProbability(float Rate, StationType Type);*/
	
	UFUNCTION(BlueprintCallable)
	void AddComplainIncreaseRate(float Rate, int32 Period);
	UFUNCTION(BlueprintCallable)
	void SetServiceData(FServiceData _ServiceData);

	UFUNCTION(BlueprintCallable)
	void NotifySpawnPassenger(StationType Type, bool IsFree);
	UFUNCTION(BlueprintCallable)
	TMap<StationType, int32> GetSpawnPassengerStatistics(int32& TotalPassenger, int32& WaitPassenger, UPARAM(DisplayName = "StationId")int32 SID = -1);
	UFUNCTION()
	void SetPassengerSpawnEnable(bool Flag);
	UFUNCTION()
	bool GetPassengerSpawnEnable() const;

	// Passenger statistics
	UFUNCTION()
	int32 GetTotalPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetFreePassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetPaidPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetArriveTotalPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetArriveFreePassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetArrivePaidPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetWaitTotalPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetWaitFreePassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetWaitPaidPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetDestroyedTotalPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetDestroyedFreePassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetDestroyedPaidPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetRideTotalPassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetRideFreePassengerCount(StationType Type = StationType::None) const;
	UFUNCTION()
	int32 GetRidePaidPassengerCount(StationType Type = StationType::None) const;

	UFUNCTION()
	void WeeklyTask();
	UFUNCTION()
	void DailyTask();

	UFUNCTION()
	void SetTransfer(bool Flag);

	UFUNCTION()
	int32 GetStationCount() const;
	UFUNCTION()
	int32 GetActiveStationCount() const;
	UFUNCTION()
	int32 GetInactiveStationCount() const;
	UFUNCTION()
	int32 GetDestroyedStationCount() const;
	UFUNCTION()
	int32 GetTypeStationCount(StationType Type) const;
	UFUNCTION()
	FStationInfo GetRandomStationInfo() const;
	UFUNCTION()
	FStationInfo GetRandomActiveStationInfo() const;
	UFUNCTION()
	FStationInfo GetRandomInactiveStationInfo() const;
	UFUNCTION()
	FStationInfo GetRandomDestroyedStationInfo() const;
	UFUNCTION()
	FStationInfo GetRandomTypeStationInfo(StationType Type) const;

private:
	AStation* GetNearestStationByType(int32 Start, StationType Type);
	void FloydWarshall();
	PathQueue PathFinding(int32 Start, StationType Type);

	void PrintPath(int32 Start, StationType Type, TQueue<int32>* Path);


protected:

	UPROPERTY(BlueprintReadOnly, Category = "Station")
	int32 MaxStationCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
	float StationSpawnRequire = 15.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Station")
	float StationSpawnCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Station")
	TArray<class AStation*> Station;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	AGridManager* GridManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	ATinyMetroGameModeBase* GameMode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	APolicy* PolicyRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class ATMSaveManager* SaveManagerRef;
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
	int32 NextStationId = 0;

	// Info Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStationInfoWidget* StationInfoWidget;

	// New station alarm widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStationSpawnBorderWidget* StationSpawnWidget;

	// Passenger statistics
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Passenger Statistics")
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Passenger Statistics")
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Passenger Statistics")
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

	// Passenger spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	bool IsPassengerSpawnEnable = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	float DefaultPassengerSpawnSpeed = 6.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	float DefaultPassengerSpawnProbability = 0.6f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	float DefaultFreePassengerSpawnProbability = 0.2f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	TMap<StationType, float> PassengerSpawnSpeed = {
		TPair<StationType, float>(StationType::Circle, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Triangle, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Rectangle, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Cross, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Rhombus, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Oval, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Diamond, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Pentagon, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Star, DefaultPassengerSpawnSpeed),
		TPair<StationType, float>(StationType::Fan, DefaultPassengerSpawnSpeed)
	};
	TMap<StationType, TArray<TPair<float, float>>> AddPassengerSpawnSpeedArr = {
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Circle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Triangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rectangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Cross,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rhombus,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Oval,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Diamond,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Pentagon,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Star,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Fan,
			TArray<TPair<float,float>>())
	};
	TMap<StationType, TArray<TPair<float, float>>> MultiplePassengerSpawnSpeedArr = {
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Circle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Triangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rectangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Cross,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rhombus,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Oval,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Diamond,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Pentagon,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Star,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Fan,
			TArray<TPair<float,float>>())
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	TMap<StationType, float> PassegnerSpawnProbability = {
		TPair<StationType, float>(StationType::Circle, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Triangle, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Rectangle, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Cross, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Rhombus, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Oval, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Diamond, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Pentagon, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Star, DefaultPassengerSpawnProbability),
		TPair<StationType, float>(StationType::Fan, DefaultPassengerSpawnProbability)
	}; 
	TMap<StationType, TArray<TPair<float, float>>> AddPassengerSpawnProbabilityArr = {
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Circle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Triangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rectangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Cross,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rhombus,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Oval,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Diamond,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Pentagon,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Star,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Fan,
			TArray<TPair<float,float>>())
	};
	TMap<StationType, TArray<TPair<float, float>>> MultiplePassengerSpawnProbabilityArr = {
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Circle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Triangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rectangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Cross,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rhombus,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Oval,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Diamond,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Pentagon,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Star,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Fan,
			TArray<TPair<float,float>>())
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	TMap<StationType, float> FreePassegnerSpawnProbability = {
		TPair<StationType, float>(StationType::Circle, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Triangle, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Rectangle, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Cross, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Rhombus, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Oval, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Diamond, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Pentagon, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Star, DefaultFreePassengerSpawnProbability),
		TPair<StationType, float>(StationType::Fan, DefaultFreePassengerSpawnProbability)
	};
	TMap<StationType, TArray<TPair<float, float>>> AddFreePassengerSpawnProbabilityArr = {
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Circle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Triangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rectangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Cross,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rhombus,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Oval,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Diamond,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Pentagon,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Star,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Fan,
			TArray<TPair<float,float>>())
	};
	TMap<StationType, TArray<TPair<float, float>>> MultipleFreePassengerSpawnProbabilityArr = {
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Circle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Triangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rectangle,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Cross,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Rhombus,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Oval,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Diamond,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Pentagon,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Star,
			TArray<TPair<float,float>>()),
		TPair<StationType, TArray<TPair<float, float>>>(
			StationType::Fan,
			TArray<TPair<float,float>>())
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

	// Save data
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FStationSpawnData> StationSpawnLog;

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
