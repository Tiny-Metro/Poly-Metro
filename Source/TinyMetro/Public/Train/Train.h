// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/TrainTemplate.h"
#include "Subtrain.h"
#include "TrainAiController.h"
#include "Train.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class TINYMETRO_API ATrain : public ATrainTemplate
{
	GENERATED_BODY()
	
public:
	ATrain();

public:
	virtual void Test() override;

	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	// TrainTemplate override function
	virtual FVector GetNextTrainDestination(FVector CurLocation) override;
	virtual void SetTrainMaterial(class ALane* Lane) override;
	virtual void Upgrade() override;
	virtual bool IsPassengerSlotFull() override;

	UFUNCTION(BlueprintCallable)
	void SetSubtrain(UPARAM(DisplayName = "Subtrains")ASubtrain* T);
	UFUNCTION(BlueprintCallable)
	void ServiceStart(FVector StartLocation, class ALane* Lane, class AStation* Destination);
	UFUNCTION()
	void ActiveMoveTest(); 
	UFUNCTION()
	void GetOnPassenger(class AStation* Station);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<ASubtrain*> Subtrains;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	FVector LocationAtPreTick;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	ATrainAiController* AiControllerRef;

protected:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapVolume;
	
	UPROPERTY()
	FLatentActionInfo RideAction;
	UPROPERTY()
	FTimerHandle RideHandle;
	UPROPERTY()
	int32 RideCount = 0;
	UPROPERTY()
	int32 PassengerIndex = 0;

	TArray<FVector> PassengerMeshPosition = {
		FVector(10.0f, 55.0f, 190.0f),
		FVector(10.0f, -55.0f, 190.0f),
		FVector(-100.0f, 55.0f, 190.0f),
		FVector(-100.0f, -55.0f, 190.0f),
		FVector(-210.0f, 55.0f, 190.0f),
		FVector(-210.0f, -55.0f, 190.0f),
		FVector(10.0f, 55.0f, 190.0f),
		FVector(10.0f, 55.0f, 190.0f)
	};

	TArray<FVector> PassengerMeshPositionUpgrade = {
		FVector(70.0f, 55.0f, 190.0f),
		FVector(70.0f, -55.0f, 190.0f),
		FVector(-50.0f, 55.0f, 190.0f),
		FVector(-50.0f, -55.0f, 190.0f),
		FVector(-160.0f, 55.0f, 190.0f),
		FVector(-160.0f, -55.0f, 190.0f),
		FVector(-270.0f, 55.0f, 190.0f),
		FVector(-270.0f, -55.0f, 190.0f)
	};

	// Test
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle TestTimer;

private:
	FTimerDelegate RideDelegate;
};
