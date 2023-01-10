// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StationType.h"
#include "Passenger.h"
#include "../SaveSystem/TMSaveManager.h"
#include "../GridGenerator/GridCellData.h"
#include "Station.generated.h"

class AStationManager;

UCLASS()
class TINYMETRO_API AStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStation();

	friend class ATMSaveManager;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetStationId(int32 Id);
	UFUNCTION(BlueprintCallable)
	void SetStationType(StationType Type);
	UFUNCTION(BlueprintCallable)
	void SetGridCellData(FGridCellData GridCellData);
	UFUNCTION(BlueprintCallable)
	void CalculateComplain();
	UFUNCTION(BlueprintCallable)
	void ActivateStation();
	UFUNCTION(BlueprintCallable)
	StationType GetStationType() const;
	UFUNCTION(BlueprintCallable)
	FGridCellData GetCurrentGridCellData() const;

	UFUNCTION(BlueprintCallable)
		void LoadStationValue(FStationValuesStruct StationValues);

	void DecreaseComplain(double ReduceRate);
	void DecreaseComplain(int32 ReduceValue);

	int32 GetComplain() const;

protected:
	void PassengerSpawnRoutine();
	void SpawnPassenger();
	double GetPassengerSpawnProbability();
	void ComplainRoutine();
	void UpdatePassengerMesh();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	AStationManager* StationManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	int32 ComplainMax = 10000;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	int32 ComplainFromInactive = 1000;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	int32 ComplainFromPassenger = 500;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	int32 ComplainCurrent = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	int32 ComplainPassengerNum = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Complain")
	int32 ComplainSpawnDay = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	int32 PassengerSpawnRequire = 6000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	int32 PassengerSpawnPerSec = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	int32 PassengerSpawnCurrent = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	double PassengerSpawnProbability = 0.6;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Passenger")
	TArray<double> PassengerSpawnProbabilityVariable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 StationId = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 Daytime = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 SpawnDay = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	bool IsActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	StationType StationTypeValue = StationType::Circle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	FGridCellData CurrentGridCellData;
	UPROPERTY(BlueprintReadOnly, Category = "TimerRoutine")
	FTimerHandle TimerSpawnPassenger;
	UPROPERTY(BlueprintReadOnly, Category = "TimerRoutine")
	FTimerHandle TimerComplain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	TArray<UPassenger*> Passenger;

protected:
	// Station meshses
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshInner = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* MeshOuter = nullptr;

	// Material
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material Current")
	UMaterialInstanceDynamic* DynamicMaterialInner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material Current")
	UMaterialInstanceDynamic* DynamicMaterialOuter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterial* MaterialActiveInner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterial* MaterialActiveOuter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterial* MaterialInactiveInner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterial* MaterialInactiveOuter;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterial* MaterialDestroyedInner;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Material")
	UMaterial* MaterialDestroyedOuter;
};
