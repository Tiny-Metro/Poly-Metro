// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TinyMetroPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ATinyMetroPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATinyMetroPlayerController(const FObjectInitializer& ObjectInitializer);


	UFUNCTION(BlueprintCallable)
	AActor* ConvertMousePositionToWorldLocation(FVector& WorldLocation, const TArray<AActor*>& TraceIgnoreActor);
	
	// Check cancel selected lane
	UFUNCTION()
	void SingleInputRelease();

protected:
	UPROPERTY()
	class ALaneManager* LaneManagerRef;
};
