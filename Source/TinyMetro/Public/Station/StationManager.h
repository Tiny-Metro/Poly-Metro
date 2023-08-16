// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../GridGenerator/GridManager.h"
#include "../GameModes/TinyMetroGameModeBase.h"
#include "Station.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "../Timer/Timestamp.h"
#include "StationInfo.h"
#include "StationSpawnData.h"
#include "PathQueue.h"
#include "AdjList.h"
#include "StationManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStationSpawnTask);

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

	UFUNCTION()
	void InitStationInfoWidget();
	UFUNCTION()
	UStationInfoWidget* GetStationInfoWidget() const;
	UFUNCTION()
	void InitStationSpawnWidget();

	UFUNCTION(BlueprintCallable)
	AStation* GetNearestStation(FVector CurrentLocation, class ALane* LaneRef);
	UFUNCTION(BlueprintCallable)
	void SpawnStation(FGridCellData GridCellData, StationType Type, int32 Id = -1, FTimestamp Timestamp = FTimestamp());
	UFUNCTION(BlueprintCallable)
	void DestroyRandomStation();
	UFUNCTION(BlueprintCallable)
	StationType GetRandomStationType(); 
	UFUNCTION(BlueprintCallable)
	static StationType StationTypeFromString(FString Str, bool& Success);
	UFUNCTION(BlueprintCallable)
	static FString StationTypeToString(StationType Type, bool& Success);

	UFUNCTION()
	void AddComplainIncreaseRateByEvent(float Rate);
	UFUNCTION()
	float GetComplainIncreaseRate() const;
	
	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	bool Load();

	// Event
	UFUNCTION()
	void EventEnd();

	// Policy 
	// Broadcast by Policy actor
	UFUNCTION()
	void UpdatePolicy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void StationSpawnRoutine(float DeltaTime);
	void TestFunction();

public:	
	UFUNCTION()
	void AddPassegnerSpawnProbabilityByEvent(float Amount);
	// Calculate probability, return spawn or not
	UFUNCTION()
	bool CalculatePassegnerSpawnProbability() const;
	// Calculate free probability, return spawn or not
	UFUNCTION()
	bool CalculateFreePassegnerSpawnProbability() const;
	UFUNCTION()
	void AddPassengerDestinationTypeWeight(StationType Type, float Amount);
	UFUNCTION()
	StationType CalculatePassengerDestination(StationType Except) const;
	float GetComplainAverage();


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
	UFUNCTION(BlueprintCallable)
	TArray<FStationInfo> GetAllStationInfo();

	PathQueue GetShortestPath(int32 Start, StationType Type);

	// Passenger Spawn
	UFUNCTION()
	float GetDefaultPassengerSpawnSpeed() const;
	UFUNCTION()
	float GetDefaultPassengerSpawnProbability() const;
	UFUNCTION()
	float GetDefaultFreePassengerSpawnProbability() const;
	UFUNCTION()
	float GetPassengerSpawnSpeed(StationType Type) const;

	UFUNCTION()
	void SetPassengerSpawnEnable(bool Flag);
	UFUNCTION()
	bool GetPassengerSpawnEnable() const;

	UFUNCTION()
	void WeeklyTask();
	UFUNCTION()
	void DailyTask();

	UFUNCTION()
	void SetTransfer(bool Flag);

private:
	AStation* GetNearestStationByType(int32 Start, StationType Type);
	void FloydWarshall();
	PathQueue PathFinding(int32 Start, StationType Type);
	void PrintPath(int32 Start, StationType Type, TQueue<int32>* Path);

protected:
	UPROPERTY()
	UObject* StationBlueprintClass = nullptr;
	UPROPERTY()
	UBlueprint* GeneratedStationBlueprint = nullptr;
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
	class APolicy* PolicyRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class ATimer* TimerRef;
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
	
	// Passenger spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	float PassengerSpawnProbability = 0.6f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	float AdditionalPassengerSpawnProbabilityByEvent = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	float AdditionalPassengerSpawnProbabilityByPolicy = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	TMap<StationType, float> PassengerDestinationTypeWeight = {
		TPair<StationType, float>(StationType::Circle, 1.0f),
		TPair<StationType, float>(StationType::Triangle, 1.0f),
		TPair<StationType, float>(StationType::Rectangle, 1.0f),
		TPair<StationType, float>(StationType::Cross, 1.0f),
		TPair<StationType, float>(StationType::Rhombus, 1.0f),
		TPair<StationType, float>(StationType::Oval, 1.0f),
		TPair<StationType, float>(StationType::Diamond, 1.0f),
		TPair<StationType, float>(StationType::Pentagon, 1.0f),
		TPair<StationType, float>(StationType::Star, 1.0f),
		TPair<StationType, float>(StationType::Fan, 1.0f)
	};

	// Passenger Spawn (Free)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	float FreePassengerSpawnProbability = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	float FreePassengerSpawnProbabilityByEvent = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	float FreePassengerSpawnProbabilityByPolicy = 0.0f;

	// Complain
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	float ComplainIncreaseRate = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	float ComplainIncreaseRateByPolicy = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	float ComplainIncreaseRateByEvent = 0.0f;

	// Info Widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStationInfoWidget* StationInfoWidget;

	// New station alarm
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStationSpawnBorderWidget* StationSpawnWidget;
	UPROPERTY(BlueprintAssignable)
	FStationSpawnTask StationSpawnTask;

	// Passenger spawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	bool IsPassengerSpawnEnable = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	float DefaultPassengerSpawnSpeed = 6.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	float DefaultPassengerSpawnProbability = 0.6f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger Spawn")
	float DefaultFreePassengerSpawnProbability = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ATinyMetroPlayerState* PlayerState;

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
