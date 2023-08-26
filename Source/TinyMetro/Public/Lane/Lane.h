// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LanePoint.h"
#include "MeshComponentArray.h"
#include "../GridGenerator/GridManager.h"
#include "../Train/TrainDirection.h"
#include "../Station/StationManager.h"
#include "BridgeTunnel/BridgeTunnelManager.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "LaneInterface.h"
#include "Lane.generated.h"
class ALaneManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPopHandleCalled, bool, IsUp);

UCLASS(Blueprintable)
class TINYMETRO_API ALane : public AActor, public ILaneInterface
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
	static FOnPopHandleCalled OnPopHandleCalled;
	UFUNCTION(BlueprintCallable)
	virtual void PopHandle(bool isUp) override;
	UFUNCTION()
	void OnOtherLanePopHandleCalled(bool isUp);

	bool CurrentlyPoppingLane;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<FLanePoint> LaneArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATinyMetroGameModeBase* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridManager* GridManagerRef;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AStationManager* StationManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	ALaneManager* LaneManagerRef;
	//class ALaneManager* LaneManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATrainManager* TrainManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATinyMetroPlayerState* TinyMetroPlayerState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ABridgeTunnelManager* BTMangerREF;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATMSaveManager* SaveManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AStatisticsManager* StatisticsManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	USplineComponent* LaneSpline;

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere)
	int32 LaneId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> LaneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LaneDefaultMaterialPath = "Material'/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial'";
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString LanedHandleMeshPath = TEXT("StaticMesh'/Game/Lane/UpdatedMeshMatrial/SM_LaneEdge.SM_LaneEdge'");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> RemoveLaneMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> RemoveLanePath = {
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_1.M_LaneTransparent_1'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_2.M_LaneTransparent_2'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_3.M_LaneTransparent_3'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_4.M_LaneTransparent_4'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_5.M_LaneTransparent_5'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_6.M_LaneTransparent_6'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_7.M_LaneTransparent_7'"),
		TEXT("Material'/Game/Resource/Material/Lane/Transparent/M_LaneTransparent_8.M_LaneTransparent_8'")
	};

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsCircularLine = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool AlreadyDeleted = false;

	UPROPERTY(BlueprintReadOnly)
	FTimerHandle SpawnTrainCheckTimer;

	UPROPERTY(BlueprintReadWrite)
	bool IsStartHandleEditing = false;

	UPROPERTY(BlueprintReadWrite)
	bool IsEndHandleEditing = false;


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

	UFUNCTION(BlueprintCallable)
	void StopHandleEditing();


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
	void RemoveLane();
	virtual void RemoveLane_Implementation();

public : // BlueprintNativeEvent
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ExtendLane();
	virtual void ExtendLane_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishRemovingLaneAtStart();
	virtual void FinishRemovingLaneAtStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishRemovingLaneAtEnd();
	virtual void FinishRemovingLaneAtEnd_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void FinishClearingLane();
	virtual void FinishClearingLane_Implementation();

public: //Delay Removing

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	TArray <AStation*> StationsToBeRemovedAtEnd;

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	TArray <AStation*> StationsToBeRemovedAtStart;

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	TArray <AStation*> StationPointBeforeRemovedEnd;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	TArray <AStation*> StationPointBeforeRemovedStart;

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	bool DoesStationsToBeRemovedAtStart= false;

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	bool DoesStationsToBeRemovedAtEnd = false;

	UPROPERTY(BlueprintReadWrite, VisibleAnyWhere)
	bool DoesLaneToBeRemoved = false;

	UFUNCTION(BlueprintCallable)
	bool CheckTrainsByDestination(const TArray <class AStation*>& Stations);

	UFUNCTION(BlueprintCallable)
	void NotifyTrainsOfRemoving(const TArray <class AStation*>& Stations);

	UFUNCTION(BlueprintCallable)
	void ModifyStationInfoWhenRemoving(const TArray <class AStation*>& Stations);

	UFUNCTION(BlueprintCallable)
	TArray<class AStation*> CollectEveryStations();

	UFUNCTION(BlueprintCallable)
	void MarkLaneToRemoveFromStart(int32 Index);

	UFUNCTION(BlueprintCallable)
	void MarkLaneToRemoveFromEnd(int32 Index,int32 ExStationNum);

	UFUNCTION(BlueprintCallable) //Mark All
	void MarkLaneToClear();



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

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FIntPoint> PointArray;

	UFUNCTION(BlueprintCallable)
	void SetLaneArray(const TArray<class AStation*>& NewStationPoint);


private:
	bool HasBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation);
	bool HasBendingPoint(FIntPoint Diff);

	FIntPoint FindBendingPoint(FIntPoint CurrentStation, FIntPoint NextStation);

	TArray<FIntPoint> GeneratePath(const FIntPoint& Start, const FIntPoint& End);

	void AddLanePoint(const FIntPoint& Point, bool IsStation, bool IsBendingPoint, TArray<FLanePoint>& TargetArray);
	void AddLanePoint(const FIntPoint& Point, bool IsStation, bool IsBendingPoint, TArray<FLanePoint>& TargetArray, int32 LanePosition);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> LaneLocation;

	UFUNCTION(BlueprintCallable)
	void SetLaneLocation();

private:
	FVector PointToLocation(const FIntPoint& Point);

public:
	UFUNCTION(BlueprintCallable)
	void ClearLanePoint();


public:
	UFUNCTION(BlueprintCallable)
	void SetLaneSpline();

public:
	UFUNCTION(BlueprintCallable)
	void HandleScaling(bool IsScaling, float Length);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	double SectionLength = 100;

public:
	UFUNCTION(BlueprintCallable)
	void HandleFullLength(bool IsFullLength);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 EndLoop;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* MeshMaterial;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* RemoveMeshMaterial;

	UFUNCTION(BlueprintCallable)
	void SetMeshMaterial();
	UFUNCTION(BlueprintCallable)
	void ChangeRemoveMaterialAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	void SetSplineMeshes();

private:
	void SetSplineMeshComponent(USplineMeshComponent* SplineMeshComponent, UStaticMesh* SplineMesh);

public:
	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
	UStaticMesh* LaneMesh;

	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
	UStaticMesh* ThroughMesh;

	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
	UStaticMeshComponent* StartHandle;

	UPROPERTY(BluePrintReadWrite, EditAnyWhere)
	UStaticMeshComponent* EndHandle;

	UFUNCTION(BlueprintCallable)
	void InitHandles();

	UFUNCTION(BlueprintCallable)
	void SetHandleMaterial();
	
	UFUNCTION(BlueprintCallable)
	void SetHandleTransform();

	UFUNCTION(BlueprintCallable)
	void SetMesh(UStaticMesh* Mesh, UStaticMesh* Through);

	UFUNCTION(BlueprintCallable)
	void RemoveLaneFromStart(int32 Index);

	UFUNCTION(BlueprintCallable)
	void ClearSplineMeshAt(int32 Index);

	UFUNCTION(BlueprintCallable)
	void RemoveLaneFromEnd(int32 Index, int32 ExStationNum);

	UFUNCTION(BlueprintCallable)
	void ExtendStart(AStation* NewStation);

	UFUNCTION(BlueprintCallable)
	void ExtendEnd(AStation* NewStation);

private:
	bool IsStationsValid(const TArray<class AStation*>& NewStationPoint);

private:
	void DisconnectBT(TArray<TArray<FIntPoint>> Area, GridType Type);
	void ConnectBT(TArray<TArray<FIntPoint>> Area, GridType Type);

	TArray<TArray<FIntPoint>> GetArea(const TArray<FLanePoint>& LaneBlock, GridType Type);

	void SetArea(const TArray<FIntPoint>& Points, TArray<TArray<FIntPoint>>& AreaArray);

	bool IsBuildble(TArray<FLanePoint>& LaneBlock);
	TArray<TArray<FIntPoint>> GetConnectorArea(TArray<FLanePoint>& LaneBlock, GridType type);
	int32 GetRequiredConnector(TArray<TArray<FIntPoint>>& AreaArray, GridType type);
//Helper Functions
	ConnectorType GetConnectorType(GridType Type);
private: 
	// Sets coord, bend, through,, lane position etc
	TArray<FLanePoint> GetLanePath(AStation* StartStation, AStation* EndStation);
	TArray<FLanePoint> GetLanePathByPoint(FIntPoint StartStation, FIntPoint EndStation);
	void GetLaneArrays(FIntPoint StartStationCoord, FIntPoint AddedStationCoord, TArray<FLanePoint>& PreLaneArray);
	void SetMeshByIndex(int32 StartIndex, int32 LastIndex);
	void SetSplineMeshComponent(FVector StartPos, FVector StartTangent, FVector EndPos, FVector EndTangent, int32 Index);
	FVector LineIntersection(FVector A, FVector B, FVector C, FVector D);

	float CalculateOffset(int32 LanePosition);
	FVector CalculateLineDirection(FVector Vector1, FVector Vector2);
	FVector CalculatePerpendicular(FVector LineDirection, float Offset, float off);
	FVector ChangePerpendicularToStandard(FVector Perpendicular);

	void UpdateLocationAndSpline();
	void GetLaneArray(const TArray<class AStation*>& NewStationPoint, TArray<FLanePoint>& PreLaneArray);
public:
	UFUNCTION(BlueprintCallable)
	bool CheckExtendable(FIntPoint StartingStationCoordinate, FIntPoint AddedStationCoordinate);
	void InitLaneSpline();

//	bool IsBuildble();


public: //Save
	UFUNCTION()
	void Save();

	UFUNCTION()
	bool Load();

	void SpawnLaneMesh();

private:
	bool HasSaveFile = false;

public:
	void SetHasSaveFile(bool hasSave);

protected: //Statistics
	UFUNCTION(BlueprintCallable)
	void SubTotalLaneCount();

	UFUNCTION(BlueprintCallable)
	void AddTotalModifyAndDeleteCount();

	UFUNCTION(BlueprintCallable)
	void AddModifyAndReduceCountInEachLane();
};