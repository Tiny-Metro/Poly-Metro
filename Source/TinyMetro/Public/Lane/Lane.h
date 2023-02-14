// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LanePoint.h"
#include "../GridGenerator/GridManager.h"
#include "../Train/TrainDirection.h"
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

	UPROPERTY(BlueprintReadWrite)
	int32 LaneNum;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridManager* GridManagerRef;

public:
	UPROPERTY(BlueprintReadWrite, Category="Lane")
	TArray<FIntPoint> StationPoint;

	TArray<FLanePoint> tmpLaneArray0;  // set station point to laneArray

	TArray<FLanePoint> tmpLaneArray1; // TODO





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
	FVector GetNextLocation(FVector CurLocation, TrainDirection Direction) const;

	UFUNCTION(BlueprintCallable)
	TrainDirection SetDirectionInit(class AStation* Station, FVector CurLocation) const;

	UFUNCTION(BlueprintCallable)
	void SpawnTrain();
};
