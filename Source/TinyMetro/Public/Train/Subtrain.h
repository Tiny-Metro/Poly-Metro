// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/TrainTemplate.h"
#include "Subtrain.generated.h"


/**
 * 
 */
UCLASS(Config = Game)
class TINYMETRO_API ASubtrain : public ATrainTemplate
{
	GENERATED_BODY()

	friend class ATrain;

public:
	ASubtrain();

	virtual void Tick(float DeltaTime) override;
	virtual void Test() override;
	virtual void SetTrainMaterial(class ALane* Lane) override;
	virtual void UpdatePassengerSlot() override;
	virtual void DespawnTrain() override;
	virtual void ServiceStart(FVector StartLocation, class ALane* Lane, UPARAM(DisplayName = "Destination")class AStation* D) override;
	virtual void UpdateTrainMesh() override;
	virtual void Upgrade() override;
	virtual bool CanUpgrade() const override;

	virtual void OnReleasedLogic() override;
	// TrainTemplate override function
	//virtual FVector GetNextTrainPosition() override;

	UFUNCTION(BlueprintCallable)
	void SetOwnerTrainId(int32 TID);
	UFUNCTION(BlueprintCallable)
	int32 GetOwnerTrainId() const;
	UFUNCTION(BlueprintCallable)
	void SetDistanceFromTrain(float Dist);
	UFUNCTION(BlueprintCallable)
	void DetachFromTrain();
	UFUNCTION(BlueprintCallable)
	void AttachToTrain(class ATrain* Train);
	UFUNCTION(BlueprintCallable)
	void SetIndex(int32 Idx);
	UFUNCTION(BlueprintCallable)
	int32 GetIndex() const;

	// Weekly tasks
	virtual void WeeklyTask() override;

	// Save & Load
	virtual void Save();
	virtual bool Load();
	virtual void FinishLoad();

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Subtrain")
	int32 OwnerTrainId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Subtrain")
	class ATrain* OwnerTrainRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Subtrain")
	int32 Index = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	class ASubtrainAiController* AiControllerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float DistanceFromTrain;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Subtrain")
	bool IsAttached = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FVector> PassengerMeshPosition = {
		FVector(110.0f, 55.0f, 190.0f),
		FVector(110.0f, -55.0f, 190.0f),
		FVector(0.0f, 55.0f, 190.0f),
		FVector(0.0f, -55.0f, 190.0f),
		FVector(-110.0f, 55.0f, 190.0f),
		FVector(-110.0f, -55.0f, 190.0f),
		FVector(-220.0f, 55.0f, 190.0f),
		FVector(-220.0f, -55.0f, 190.0f)
	};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FVector> PassengerMeshPositionUpgrade = {
		FVector(160.0f, 55.0f, 190.0f),
		FVector(160.0f, -55.0f, 190.0f),
		FVector(50.0f, 55.0f, 190.0f),
		FVector(50.0f, -55.0f, 190.0f),
		FVector(-60.0f, 55.0f, 190.0f),
		FVector(-60.0f, -55.0f, 190.0f),
		FVector(-170.0f, 55.0f, 190.0f),
		FVector(-170.0f, -55.0f, 190.0f)
	};

	// Used when main train non-upgrade
	// Index 0 : Non upgrade mesh
	// Index 1 : Upgrade mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FString> SubtrainMeshPath = {
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainShort.SM_SubtrainShort'"),
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainLong.SM_SubtrainLong'")
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMesh*> SubtrainMesh;

	// Used when main train upgrade
	// Index 0 : Non upgrade mesh
	// Index 1 : Upgrade mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FString> UpgradeSubtrainMeshPath = {
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainShort_Upgrade.SM_SubtrainShort_Upgrade'"),
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainLong_Upgrade.SM_SubtrainLong_Upgrade'")
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TArray<UStaticMesh*> UpgradeSubtrainMesh;
};
