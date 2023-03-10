// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StationType.h"
#include "StationState.h"
#include "Passenger.h"
#include "../SaveSystem/TMSaveManager.h"
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

	friend class ATMSaveManager;


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
	UFUNCTION(BlueprintCallable)
	void CalculateComplain();
	UFUNCTION(BlueprintCallable)
	void ActivateStation();


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
	void SetPolicy(APolicy* Policy);
	UFUNCTION(BlueprintCallable)
	void UpdateStationMesh();
	UFUNCTION(BlueprintCallable)
	void LoadStationValue(FStationValuesStruct StationValues);
	UFUNCTION(BlueprintCallable)
	bool IsValidLane(int32 LId) const;

	//UFUNCTION()
	/*Return passenger at Index
	Key is passenger's pointer, set nullptr when passenger don't want ride
	Value is bool of Index's validation. return true when index is valid
	*/
	TPair<UPassenger*, bool> GetOnPassenger(int32 Index);
	void GetOffPassenger(class UPassenger* P);
    
	void AddPassengerSpawnProbability(float rate, int32 dueDate);

	void DecreaseComplain(double ReduceRate);
	void DecreaseComplain(int32 ReduceValue);

	int32 GetComplain() const;
	TArray<int32> GetLanes();
	void SetLanes(int32 AdditionalLaneId);

	FStationInfo GetStationInfo();
	void SetStationInfo(int32 Id, StationType Type);

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
	float ComplainMax = 100;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	float ComplainFromInactive = 10;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	float ComplainFromPassenger = 5;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Complain")
	float ComplainCurrent = 0;
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
	double AdditionalPassengerSpawnProbability = 1.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 StationId = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 Daytime = 0;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	APolicy* Policy;
	UPROPERTY(BlueprintReadOnly, Category = "TimerRoutine")
	FTimerHandle TimerSpawnPassenger;
	UPROPERTY(BlueprintReadOnly, Category = "TimerRoutine")
	FTimerHandle TimerComplain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Passenger")
	TArray<UPassenger*> Passenger;
	UPROPERTY(BlueprintReadWrite)
	TArray<int32> Lanes;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	FStationInfo StationInfo;


	UPROPERTY(BlueprintReadWrite, Category = "Passenger")
	int32 MaxPassengerSpawn = 15;
	UPROPERTY(BlueprintReadWrite, Category = "Passenger")
	FVector PassengerMeshDefaultPosition = FVector(180.0f, 0.0f, 10.0f);
	UPROPERTY(BlueprintReadWrite, Category = "Passenger")
	float PassengerX_Distance = 90;
	UPROPERTY(BlueprintReadWrite, Category = "Passenger")
	float PassengerY_Distance = 45;

protected:
	// Station meshses
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* StationMeshComponent = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> StationMesh;

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

	// Mesh, Material paths (Station)
	UPROPERTY()
	TArray<FString> StationMeshPath = {
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
};
