// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainTemplate.h"
#include "TrainType.h"
#include "TrainManager.generated.h"

UCLASS()
class TINYMETRO_API ATrainManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void AddTrain(ATrainTemplate* Train);
	UFUNCTION(BlueprintCallable)
	void RemoveTrain(ATrainTemplate* Train);
	UFUNCTION(BlueprintCallable)
	void SetTrainId(ATrainTemplate* Train);
	UFUNCTION(BlueprintCallable)
	ATrainTemplate* GetTrainById(int32 TrainId, TrainType& Type);
	UFUNCTION(BlueprintCallable)
	ATrain* GetNearestTrain(FVector CurrentLocation, class ALane* LaneRef);
	UFUNCTION(BlueprintCallable)
	TArray<ATrainTemplate*> GetAllTrain();

	UFUNCTION()
	void InitTrainMaterial();
	UFUNCTION()
	TArray<UMaterial*> GetTrainMaterial() const;

	UFUNCTION()
	void InitPassengerMaterial();
	UFUNCTION()
	TArray<UMaterial*> GetPassengerMaterial() const;

	UFUNCTION()
	void RefreshTrainArray();
	
	UFUNCTION(BlueprintCallable)
	void TrainDeferredDespawn(struct FStationInfo Station, class ALane* Lane);
	UFUNCTION(BlueprintCallable)
	TArray<ATrainTemplate*> GetStationsByDestination(struct FStationInfo Station, class ALane* Lane);
	UFUNCTION(BlueprintCallable)
	int32 GetStationCountByOrigin(struct FStationInfo Origin, class ALane* Lane);
	UFUNCTION(BlueprintCallable)
	int32 GetStationCountByDestination(struct FStationInfo Destination, class ALane* Lane);

	UFUNCTION()
	int32 GetTrainCount(int32 LaneId = -1) const;
	UFUNCTION()
	int32 GetTrainCountFilterByUpgrade(bool Upgrade, int32 LaneId = -1) const;
	UFUNCTION()
	int32 GetMainTrainCount(int32 LaneId = -1) const;
	UFUNCTION()
	int32 GetMainTrainCountFilterByUpgrade(bool Upgrade, int32 LaneId = -1) const;
	UFUNCTION()
	int32 GetSubTrainCount(int32 LaneId = -1) const;
	UFUNCTION()
	int32 GetSubTrainCountFilterByUpgrade(bool Upgrade, int32 LaneId = -1) const;

	// Spawn Train, Subtrain (Call by Load)
	UFUNCTION()
	void SpawnTrain(int32 TrainId, FVector SpawnLocation);
	UFUNCTION()
	void SpawnSubtrain(int32 TrainId, int32 OwnerId, FVector SpawnLocation);

	// Get upgrade cost
	UFUNCTION()
	float GetCostUpgradeTrain() const;
	UFUNCTION()
	float GetCostUpgradeSubtrain() const;

	// Call when (sub)train upgrade
	UFUNCTION()
	void ReportTrainUpgrade();
	UFUNCTION()
	void ReportSubtrainUpgrade();

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	bool Load();


protected:
	// Train, Subtrain asset
	UPROPERTY()
	UObject* TrainBlueprintClass = nullptr;
	UPROPERTY()
	UBlueprint* GeneratedTrainBlueprint = nullptr;
	UPROPERTY()
	UObject* SubtrainBlueprintClass = nullptr;
	UPROPERTY()
	UBlueprint* GeneratedSubtrainBlueprint = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	class ATMSaveManager* SaveManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TArray<ATrainTemplate*> Trains;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 NextTrainId = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> TrainMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> PassengerMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTrainInfoWidget* TrainInfoWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CostUpgradeTrain = 200.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CostUpgradeSubtrain = 200.0f;
};
