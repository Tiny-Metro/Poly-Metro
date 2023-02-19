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
	virtual bool SetTrainMaterial(int32 LaneNumber) override;
	virtual void Upgrade() override;

	UFUNCTION(BlueprintCallable)
	void SetSubtrain(UPARAM(DisplayName = "Subtrains")ASubtrain* T);
	UFUNCTION(BlueprintCallable)
	void ServiceStart(FVector StartLocation, class ALane* Lane, class AStation* Destination);


protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


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
	

	// Test
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FTimerHandle TestTimer;
};
