// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TinyMetroCameraSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTinyMetroCameraSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FVector Location;
	UPROPERTY(BlueprintReadWrite)
	FRotator ParentRotation;
	UPROPERTY(BlueprintReadWrite)
	FRotator SpringArmRotation;
	UPROPERTY(BlueprintReadWrite)
	float CameraDistance;
};
