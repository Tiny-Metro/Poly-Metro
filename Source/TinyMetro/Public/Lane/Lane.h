// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LanePoint.h"
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
	UPROPERTY(BlueprintReadWrite)
	TArray<FLanePoint> LaneArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridManager* GridManagerRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AStationManager* StationManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ALaneManager* LaneManagerRef;

protected:
	UPROPERTY(BlueprintReadOnly)
	int32 LaneId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> LaneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LaneDefaultMaterialPath = "Material'/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial'";

	UPROPERTY(BlueprintReadOnly)
	bool IsCircularLine = false;

public:
	UFUNCTION(BlueprintCallable)
	int32 GetLaneId() const;
	UFUNCTION(BlueprintCallable)
	void SetLaneId(int _LaneId);
	UFUNCTION(BlueprintCallable)
	void InitLaneMaterial(TArray<UMaterial*> Materials);

	UFUNCTION(BlueprintCallable)
	bool GetIsCircularLine();
	UFUNCTION(BlueprintCallable)
	void SetIsCircularLine(bool _Circular);

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


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void ExtendLane();
	virtual void ExtendLane_Implementation();
	

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lane")
	TArray<FIntPoint> RStationPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Lane")
	TArray<class AStation* > RRStationPoint;

	UFUNCTION(BlueprintCallable)
	void RSetLaneArray(const TArray<class AStation*>& NewStationPoint);
//	void RSetLaneArray(const TArray<FIntPoint>& NewStationArray);

private:
	bool hasBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation);

	FIntPoint findBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation);

	TArray<FIntPoint> GeneratePath(const FIntPoint& Start, const FIntPoint& End);

// REFACTORING SetLaneVector
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> RLaneLocation;

	UFUNCTION(BlueprintCallable)
	void RSetLaneLocation();

private:
	FVector PointToLocation(const FIntPoint& Point);

// Refactoring clearSplineMesh
public:
	UFUNCTION(BlueprintCallable)
	void ClearSplineMesh(TArray<USplineMeshComponent*> SplineMesh);

//	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//TArray<USplineMeshComponent*> SplineMesh;

//Refactoring SetSpline
public:
	UFUNCTION(BlueprintCallable)
	void SetLaneSpline(USplineComponent* Spline);
};
