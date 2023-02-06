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
	UFUNCTION(BlueprintCallable)
	void SetTargetTrain(ATrainTemplate* T);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	ATrainTemplate* TargetTrain;
};
