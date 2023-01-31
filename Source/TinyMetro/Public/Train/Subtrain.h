// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/TrainTemplate.h"
#include "Subtrain.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ASubtrain : public ATrainTemplate
{
	GENERATED_BODY()
	
public:
	virtual void Test() override;

	// TrainTemplate override function
	virtual FVector GetNextTrainPosition() override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Subtrain")
	int32 OwnerTrainId;
};
