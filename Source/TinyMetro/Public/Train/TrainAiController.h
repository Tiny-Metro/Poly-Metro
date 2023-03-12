// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TrainAiController.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ATrainAiController : public AAIController
{
	GENERATED_BODY()

public:
	void Test();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Patrol();
	virtual void Patrol_Implementation();
	UFUNCTION(BlueprintCallable)
	void SetTrainDestination(FVector Destination);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<FVector> TestRoutine;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	FVector TrainDestination;
	
};
