// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StationType.h"
#include "StationState.h"
#include "Passenger.h"
#include "../GridGenerator/GridCellData.h"
#include "../Policy/Policy.h"
#include "StationInfo.h"
#include "Station.generated.h"

class AStationManager;

UCLASS(Config = Game)
class TINYMETRO_API AStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Station id Getter, Setter
	UFUNCTION(BlueprintCallable)
	void SetStationId(int32 Id);
	UFUNCTION(BlueprintCallable)
	int32 GetStationId() const;

	// Station complain function
	UFUNCTION()
	void ComplainRoutine();
	UFUNCTION(BlueprintCallable)
	void SetActivate(bool Flag);
	UFUNCTION()
	void UpdateComplainMesh();
	UFUNCTION()
	void InitComplainGauge();
	UFUNCTION()
	void SetComplainGauge(float Per);
	UFUNCTION()
	void SetComplainIncreaseEnable(bool Flag);
	UFUNCTION()
	void ScaleComplain(float Rate);
	UFUNCTION()
	void AddComplain(float Value, bool IsFixedValue = false);
	UFUNCTION()
	float GetComplain() const;

	// Station state Getter, Setter
	UFUNCTION(BlueprintCallable)
	StationState GetStationState() const;
	UFUNCTION(BlueprintCallable)
	void SetStationState(UPARAM(DisplayName = "State")StationState S);

	// Station type Getter, Setter
	UFUNCTION(BlueprintCallable)
	StationType GetStationType() const;
	UFUNCTION(BlueprintCallable)
	void SetStationType(StationType Type);

	// Station grid data Getter, Setter
	UFUNCTION(BlueprintCallable)
	FGridCellData GetCurrentGridCellData() const;
	UFUNCTION(BlueprintCallable)
	void SetGridCellData(FGridCellData GridCellData);

	UFUNCTION(BlueprintCallable)
	void UpdateStationMesh();

	// Daily, Weekly tasks
	UFUNCTION()
	void WeeklyTask();
	UFUNCTION()
	void DailyTask();

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

	// Lane
	UFUNCTION(BlueprintCallable)
	bool IsValidLane(int32 LId) const;
	UFUNCTION(BlueprintCallable)
	void RemoveLane(int32 LId);

	// Passenger
	UFUNCTION()
	void PassengerSpawnRoutine(float DeltaTime);
	UFUNCTION()
	void SpawnPassenger(StationType Destination);
	UFUNCTION()
	void DespawnPassenger(StationType Destination);
	UFUNCTION()
	void DespawnRandomPassenger();
	UFUNCTION()
	void SetPassengerSpawnEnable(bool Flag);
	UFUNCTION()
	bool GetPassengerSpawnEnable() const;
	UFUNCTION()
	int32 GetWaitPassenger() const;
	
	/*Return passenger at Index
	Key is passenger's pointer, set nullptr when passenger don't want ride
	Value is bool of Index's validation. return true when index is valid
	*/
	FPassenger GetOnPassenger(int32 Index, class ATrainTemplate* Train);
	void GetOffPassenger(FPassenger P, class ATrainTemplate* Train);
 
	TArray<int32> GetLanes();
	void SetLanes(int32 AdditionalLaneId);

	UFUNCTION()
	void SetTransfer(bool Flag);

	FStationInfo GetStationInfo();
	void SetStationInfo(int32 Id, StationType Type);

	// Upgrade
	UFUNCTION(BlueprintCallable)
	void Upgrade();
	UFUNCTION(BlueprintCallable)
	bool CanUpgrade() const;

	// Init widget
	UFUNCTION()
	void SetInfoWidget(class UStationInfoWidget* Widget);

	// Click & Release
	UFUNCTION()
	virtual void StationOnPressed(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
	virtual void StationOnReleased(AActor* Target, FKey ButtonPressed);

	// Touch
	UFUNCTION()
	virtual void StationTouchBegin(ETouchIndex::Type FingerIndex, AActor* TouchedActor);
	UFUNCTION()
	virtual void StationTouchEnd(ETouchIndex::Type FingerIndex, AActor* TouchedActor);

	// Info widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStationInfoWidget* StationInfoWidget;

	// New Station
	UFUNCTION()
	void OffSpawnAlarm();

	// Event
	UFUNCTION()
	void EventEnd();

	// Policy 
	// Broadcast by Policy actor
	UFUNCTION()
	void UpdatePolicy();

protected:
	void SpawnPassenger();
	void UpdatePassengerMesh();

protected:
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	APolicy* PolicyRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class ATimer* TimerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class ATinyMetroPlayerState* PlayerStateRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	AStationManager* StationManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class ALaneManager* LaneManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 MaintenanceCost = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	FServiceData PolicyServiceLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	float ComplainMax = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	float ComplainFromInactive = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	float ComplainFromPassenger = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	int32 ComplainPassengerNum = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	int32 ComplainSpawnDay = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	bool IsComplainIncreaseEnable = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	float PassengerSpawnRequire = 6.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	float PassengerSpawnCurrent = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	float PassengerSpawnSpeed = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	bool IsPassengerSpawnEnable = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 SpawnDay = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	bool IsActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	StationState State = StationState::Inactive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	StationType StationTypeValue = StationType::Circle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	FGridCellData CurrentGridCellData;
	UPROPERTY(BlueprintReadOnly, Category = "TimerRoutine")
	FTimerHandle TimerSpawnPassenger;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	TArray<FPassenger> Passenger;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	TArray<int32> Lanes;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	FStationInfo StationInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	bool TransferStation = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	int32 Fare = 2;

	// Touch
	UPROPERTY(BlueprintReadWrite)
	float TouchTime = 0.0f;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	float LongClickInterval = 0.5f;

	// Passenger mesh
	UPROPERTY(BlueprintReadWrite, Category = "Passenger mesh")
	int32 MaxPassengerSpawn = 15;
	UPROPERTY(BlueprintReadWrite, Category = "Passenger mesh")
	FVector PassengerMeshDefaultPosition = FVector(180.0f, 0.0f, 10.0f);
	UPROPERTY(BlueprintReadWrite, Category = "Passenger mesh")
	float PassengerX_Distance = 90;
	UPROPERTY(BlueprintReadWrite, Category = "Passenger mesh")
	float PassengerY_Distance = 45;

	// Upgrade
	UPROPERTY(BlueprintReadOnly, Category = "Config")
	int32 UpgradePermissionComplainPassenger = 3;
	UPROPERTY(BlueprintReadOnly, Category = "Config")
	float UpgradeCost = 300.0f;

	// New station
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	UStaticMeshComponent* PulseComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect")
	bool SpawnAlarm = true;

protected:
	// Station meshses
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StationMeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StationComplainMeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> StationMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> StationComplainMesh;

	// Passenger meshes
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<UStaticMeshComponent*> PassengerMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> PassengerMesh;
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapVolume;

	// Material (Station)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> StationMaterialInactive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> StationMaterialActive;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> StationMaterialDestroyed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> PassengerMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInterface* ComplainMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic* ComplainDynamicMaterial;

	// Mesh, Material paths (Station)
	UPROPERTY()
	TArray<FString> StationMeshPath = {
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationNone.SM_StationNone'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCircle.SM_StationCircle'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationTriangle.SM_StationTriangle'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationRectangle.SM_StationRectangle'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCross.SM_StationCross'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationRhombus.SM_StationRhombus'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationOval.SM_StationOval'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationDiamond.SM_StationDiamond'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationPentagon.SM_StationPentagon'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationStar.SM_StationStar'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationFan.SM_StationFan'")
	};
	UPROPERTY()
	TArray<FString> StationMaterialInactivePath = {
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationInactive_Outer.M_StationInactive_Outer'"),
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationInactive_Inner.M_StationInactive_Inner'")
	};
	UPROPERTY()
	TArray<FString> StationMaterialActivePath = {
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationActive_Outer.M_StationActive_Outer'"),
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationActive_Inner.M_StationActive_Inner'")
	};
	UPROPERTY()
	TArray<FString> StationMaterialDestroyedPath = {
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationDestroyed_Outer.M_StationDestroyed_Outer'"),
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationDestroyed_Inner.M_StationDestroyed_Inner'")
	};

	// Mesh, Material paths (Passenger)
	UPROPERTY()
	TArray<FString> PassengerMeshPath = {
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerNone.SM_PassengerNone'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerCircle.SM_PassengerCircle'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerTriangle.SM_PassengerTriangle'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerRectangle.SM_PassengerRectangle'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerCross.SM_PassengerCross'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerRhombus.SM_PassengerRhombus'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerOval.SM_PassengerOval'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerDiamond.SM_PassengerDiamond'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerPentagon.SM_PassengerPentagon'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerStar.SM_PassengerStar'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerFan.SM_PassengerFan'")
	};
	UPROPERTY()
	TArray<FString> PassengerMaterialPath = {
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationInactive_Outer.M_StationInactive_Outer'"),
		TEXT("Material'/Game/Station/Asset/StationMaterial/M_StationInactive_Inner.M_StationInactive_Inner'")
	};

	// Mesh of complain gauge paths
	TArray<FString> ComplainMeshPath = {
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_NoneGauge.SM_NoneGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_CircleGauge.SM_CircleGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_TriangleGauge.SM_TriangleGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_RectangleGauge.SM_RectangleGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_CrossGauge.SM_CrossGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_RhombusGauge.SM_RhombusGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_OvalGauge.SM_OvalGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_DiamondGauge.SM_DiamondGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_PentagonGauge.SM_PentagonGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StarGauge.SM_StarGauge'"),
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_FanGauge.SM_FanGauge'"),
	};
	FString ComplainMaterialPath = TEXT("Material'/Game/Station/Asset/StationMaterial/M_Gauge.M_Gauge'");
};
