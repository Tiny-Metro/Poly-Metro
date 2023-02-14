// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lane.h"
#include "../Station/Station.h"
#include "LaneManager.generated.h"

UCLASS()
class TINYMETRO_API ALaneManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaneManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(BlueprintReadWrite)
	int32 NextLaneId = 1;

	UPROPERTY(BlueprintReadWrite)
	TArray<ALane*> Lanes;

	UPROPERTY(BlueprintReadWrite)
	bool IsPlacementValid;

	UPROPERTY(BlueprintReadWrite)
	int32 SelectedLaneNum;

public:
	UFUNCTION(BlueprintCallable)
	void CreatingNewLane(TArray <AStation*> SelectedStations);

	UFUNCTION(BlueprintCallable)
	void AddStationInLane(int CurrentLane);
	
	UFUNCTION(BlueprintCallable)
	void AddLane(ALane* Obj);
	
	UFUNCTION(BlueprintCallable)
	ALane* GetLaneById(int32 LaneId);

	
};
