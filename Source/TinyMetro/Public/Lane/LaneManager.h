// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Lane.h"
#include "../Station/Station.h"
#include "../Station/StationManager.h"
#include "LaneManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLaneSelectDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLaneCancelDelegate);

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

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	TMap<int32, ALane*> Lanes;

	UPROPERTY(BlueprintReadWrite)
	bool IsPlacementValid;

	UPROPERTY(BlueprintAssignable)
	FLaneSelectDelegate LaneSelectDelegate;

	UPROPERTY(BlueprintAssignable)
	FLaneCancelDelegate LaneCancelDelegate;

protected:
	UPROPERTY(VisibleAnywhere)
	UClass* LaneClass = nullptr;

	UPROPERTY(BlueprintReadOnly)
	bool CanAssignBridge;

	UPROPERTY(BlueprintReadOnly)
	bool CanAssignTunnel;

	UPROPERTY(BlueprintReadOnly)
	TArray<int32> NextLaneNums = {1,2,3,4,5,6,7,8};

	UPROPERTY(BlueprintReadOnly)
	class ATinyMetroGameModeBase* GameMode;

	UPROPERTY(BlueprintReadOnly)
	AStationManager* StationManagerRef;

	UPROPERTY(BlueprintReadOnly)
	class ATMSaveManager* SaveManagerRef;

	UPROPERTY(BlueprintReadOnly)
	class AStatisticsManager* StatisticsManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> LaneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSoftObjectPath> LaneMaterialPath;

	UPROPERTY(BlueprintReadOnly)
	int32 SelectedLaneNum;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	int32 UILaneNum;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	TArray<class AStation*> SelectedStations;

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

	UFUNCTION(BlueprintCallable)
	void SetUILaneNum(int32 Num);

	UFUNCTION(BlueprintCallable)
	int32 GetUILaneNum();

	UFUNCTION(BlueprintCallable)
	void SetSelectedLaneNum(int32 Num);

	UFUNCTION(BlueprintCallable)
	void AddSelectedStations(class AStation* Station);

	UFUNCTION(BlueprintCallable)
	void CancelSelectedStations();


public:
	UFUNCTION(BlueprintCallable)
	class ALane* SpawnLane();

	UFUNCTION(BlueprintCallable)
	void CreatingNewLane();

	/*UFUNCTION(BlueprintCallable)
	void AddStationInLane(int CurrentLane);*/
	
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

public:
	int32 GetPosition(FIntPoint Start, FIntPoint End);

	UFUNCTION()
	void Save();

	UFUNCTION()
	bool Load();

	UFUNCTION()
	class ALane* LoadLane(int32 LaneId);

	//Lane Statics
	UFUNCTION(BlueprintCallable)
	void CheckTransferStation();
};
