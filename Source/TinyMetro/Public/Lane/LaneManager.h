// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lane.h"
#include "../Station/Station.h"
#include "../Station/StationManager.h"
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

	UFUNCTION()
	int32 GetLaneCount() const;
	UFUNCTION()
	int32 GetRandomLaneId() const;

public:
	//UPROPERTY(BlueprintReadWrite)
	//int32 NextLaneId = 1;

	UPROPERTY(BlueprintReadWrite)
	TMap<int32, ALane*> Lanes;

	UPROPERTY(BlueprintReadWrite)
	bool IsPlacementValid;

	UPROPERTY(BlueprintReadWrite)
	int32 SelectedLaneNum;


protected:

	UPROPERTY(BlueprintReadOnly)
	bool CanAssignBridge;

	UPROPERTY(BlueprintReadOnly)
	bool CanAssignTunnel;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> NextLaneNums = {1,2,3,4,5,6,7,8};

	UPROPERTY(BlueprintReadOnly)
	AStationManager* StationManagerRef;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> LaneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSoftObjectPath> LaneMaterialPath;

public:
	UFUNCTION(BlueprintCallable)
	void SetCanAssginBridge(bool AssginBridge);

	UFUNCTION(BlueprintCallable)
	void SetCanAssginTunnel(bool AssginTunnel);

	UFUNCTION(BlueprintCallable)
	void AddNextLaneNums(int32 LaneNum);

	UFUNCTION(BlueprintCallable)
	void RemoveNextLaneNums();

	UFUNCTION(BlueprintCallable)
	void RemoveDestroyedLane(int LaneNum);



public:
	UFUNCTION(BlueprintCallable)
	void CreatingNewLane(TArray <AStation*> SelectedStations);

	UFUNCTION(BlueprintCallable)
	void AddStationInLane(int CurrentLane);
	
	UFUNCTION(BlueprintCallable)
	void AddLane(ALane* Obj);
	
	UFUNCTION(BlueprintCallable)
	ALane* GetLaneById(int32 LaneId);


	UFUNCTION()
	void InitLaneMaterial();
	UFUNCTION()
	void LaneMaterialDeferred();
	UFUNCTION()
	TArray<UMaterial*> GetLaneMaterial() const;


	UFUNCTION()
	void Save();

	UFUNCTION()
	void Load();
};
