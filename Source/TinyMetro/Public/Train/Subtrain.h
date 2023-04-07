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
	
public:
	ASubtrain();

	virtual void Test() override;
	virtual void SetTrainMaterial(class ALane* Lane) override;
	virtual void UpdatePassengerSlot() override;
	virtual void DespawnTrain() override;

	// TrainTemplate override function
	//virtual FVector GetNextTrainPosition() override;

	UFUNCTION(BlueprintCallable)
	void SetOwnerTrainId(int32 TID);
	UFUNCTION(BlueprintCallable)
	int32 GetOwnerTrainId() const;
	UFUNCTION(BlueprintCallable)
	void SetDistanceFromTrain(float Dist);

protected:
	virtual void BeginPlay() override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Subtrain")
	int32 OwnerTrainId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	class ASubtrainAiController* AiControllerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float DistanceFromTrain;

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
};
