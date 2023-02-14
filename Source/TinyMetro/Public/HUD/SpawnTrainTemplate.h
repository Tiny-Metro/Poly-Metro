// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SpawnTrainTemplate.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API USpawnTrainTemplate : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Hit Actor") AActor* ConvertMousePositionToWorldLocation(FVector& WorldLocation);
	
protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> LineTraceIgnoreActors;
};
