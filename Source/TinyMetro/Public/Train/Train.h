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
UCLASS()
class TINYMETRO_API ATrain : public ATrainTemplate
{
	GENERATED_BODY()
	
public:
	virtual void Test() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// TrainTemplate override function
	virtual FVector GetTrainNextLocation() override;

protected:

	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<ASubtrain*> Subtrains;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float TotalTravel;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	FVector LocationAtPreTick;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	ATrainAiController* ParentAiControllerRef;
};
