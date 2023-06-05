// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LanePoint.h"
#include "MeshComponentArray.h"
#include "../GridGenerator/GridManager.h"
#include "../Train/TrainDirection.h"
#include "../Station/StationManager.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Lane.generated.h"

UCLASS(Blueprintable)
class TINYMETRO_API ALane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALane();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<FLanePoint> LaneArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridManager* GridManagerRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AStationManager* StationManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ALaneManager* LaneManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATrainManager* TrainManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATinyMetroPlayerState* TinyMetroPlayerState;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 LaneId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> LaneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LaneDefaultMaterialPath = "Material'/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial'";

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsCircularLine = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool AlreadyDeleted = false;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle SpawnTrainCheckTimer;


public:
	UFUNCTION(BlueprintCallable)
	int32 GetLaneId() const;
	UFUNCTION(BlueprintCallable)
	void SetLaneId(int _LaneId);
	UFUNCTION(BlueprintCallable)
	void InitLaneMaterial(TArray<UMaterial*> Materials);

	UFUNCTION(BlueprintCallable)
	bool GetIsCircularLine() const;
	UFUNCTION(BlueprintCallable)
	void SetIsCircularLine(bool _Circular);
	
	UFUNCTION(BlueprintCallable)
	FLanePoint GetNearestLanePoint(FVector Location);
	UFUNCTION(BlueprintCallable)
	LaneDirection GetLaneShape(FVector Location);

	UFUNCTION(BlueprintCallable)
	bool GetAlreadyDeleted() const;

	UFUNCTION(BlueprintCallable)
	void SetAlreadyDeleted(bool _Delete);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Lane")
	TArray<class AStation * > StationPoint;


	UPROPERTY(BlueprintReadWrite, Category="Lane")
	TArray<FLanePoint> tmpLaneArray0;  // set station point to laneArray


	UPROPERTY(BlueprintReadWrite, Category="Lane")
	TArray<FLanePoint> tmpLaneArray1; 

	UPROPERTY(BlueprintReadWrite, Category = "Lane")
	FLanePoint TmpLanePoint0;

	UPROPERTY(BlueprintReadWrite, Category = "Lane")
	FLanePoint TmpLanePoint1;


public:
	UFUNCTION(BlueprintCallable)
	void CheckStationPoint();

	UFUNCTION(BlueprintCallable)
	void SetBendingPoint();

	bool FindBendingPoint(FIntPoint& TargetPoint,FIntPoint PointStart, FIntPoint PointEnd);

	UFUNCTION(BlueprintCallable)
	void FillLanePoint();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LaneCreating();
	virtual void LaneCreating_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InitializeNewLane();
	virtual void InitializeNewLane_Implementation();

public : // BlueprintNativeEvent
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExtendLane();
	virtual void ExtendLane_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishRemovingLaneAtStart(const TArray <class AStation*>& Stations, const int32 Index);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishRemovingLaneAtEnd(const TArray <class AStation*>& Stations, const int32 Index);
	
public: // About Train

	UFUNCTION(BlueprintCallable)
	FIntPoint GetNextLocation(class ATrainTemplate* Train, FIntPoint CurLocation, TrainDirection Direction);

	UFUNCTION(BlueprintCallable)
	TrainDirection SetDirectionInit(class AStation* Station, FIntPoint CurLocation) const;

	UFUNCTION(BlueprintCallable)
	void SpawnTrain();

	UFUNCTION(BlueprintCallable)
	AStation* GetNextStation(AStation* CurrStation, TrainDirection Direction);

	UFUNCTION(BlueprintCallable)
	void SetGridLaneStructure();

	UFUNCTION(BlueprintCallable)
	void AddAdjListDistance(AStation* First, AStation* Second);

	UFUNCTION(BlueprintCallable)
	FIntPoint GetWorldCoordinationByStationPointIndex(int32 Index);

//REFACTORING SETLANEARRAY

public:
	UPROPERTY(BlueprintReadWrite, Category = "Lane")
	TArray<FLanePoint> RLaneArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntPoint> PointArray;

	UFUNCTION(BlueprintCallable)
	void SetLaneArray(const TArray<class AStation*>& NewStationPoint);


private:
	bool hasBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation);

	FIntPoint findBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation);

	TArray<FIntPoint> GeneratePath(const FIntPoint& Start, const FIntPoint& End);

// REFACTORING SetLaneVector
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> RLaneLocation;

	UFUNCTION(BlueprintCallable)
	void SetLaneLocation();

private:
	FVector PointToLocation(const FIntPoint& Point);

// Refactoring clearSplineMesh
public:
	UFUNCTION(BlueprintCallable)
	void ClearLanePoint();

//Refactoring SetSpline
public:
	UFUNCTION(BlueprintCallable)
	void SetLaneSpline(USplineComponent* Spline);

//Refactoring HandleScaling
public:
	UFUNCTION(BlueprintCallable)
	void HandleScaling(bool IsScaling, float SectionLength);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double RSectionLength;

//Refactoring HandleFullLength
public:
	UFUNCTION(BlueprintCallable)
	void HandleFullLength(bool IsFullLength, USplineComponent* Spline);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 EndLoop;

// Refactoring AddSplineMeshComponent
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* MeshMaterial;

	UFUNCTION(BlueprintCallable)
	void SetMeshMaterial();


//Refactoring AddingSplineMesh in other way - 
	UFUNCTION(BlueprintCallable)
	void SetSplineMeshes(USplineComponent* Spline);

private:
	void SetSplineMeshComponent(USplineMeshComponent* SplineMeshComponent, UStaticMesh* SplineMesh);

	public:
//ReFactoring Setting Mesh & Remove
		UPROPERTY(BluePrintReadWrite, EditAnyWhere)
		UStaticMesh* LaneMesh;

		UPROPERTY(BluePrintReadWrite, EditAnyWhere)
		UStaticMesh* ThroughMesh;

		UFUNCTION(BlueprintCallable)
		void SetMesh(UStaticMesh* Mesh, UStaticMesh* Through);

		UFUNCTION(BlueprintCallable)
		void RemoveLaneFromStart(int32 Index, USplineComponent* Spline);

		UFUNCTION(BlueprintCallable)
		void ClearSplineMeshAt(int32 Index);

		UFUNCTION(BlueprintCallable)
		void RemoveLaneFromEnd(int32 Index, int32 ExStationNum, USplineComponent* Spline);

		UFUNCTION(BlueprintCallable)
		void ExtendStart(AStation* NewStation, USplineComponent* Spline);

		UFUNCTION(BlueprintCallable)
		void ExtendEnd(AStation* NewStation, USplineComponent* Spline);
};