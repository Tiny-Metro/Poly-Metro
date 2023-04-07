// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TrainTemplate.h"
#include "SubtrainAiController.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ASubtrainAiController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Follow();
	virtual void Follow_Implementation();
	UFUNCTION(BlueprintCallable)
	void SetTargetTrain(ATrainTemplate* T);
	UFUNCTION(BlueprintCallable)
	void SetDistanceFromTrain(float Dist);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	ATrainTemplate* TargetTrain;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float DistanceFromTrain;
};
