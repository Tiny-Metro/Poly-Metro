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

public:
	//virtual void Construct() override;
	virtual void NativeConstruct() override;

protected:
	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName = "Hit Actor") AActor* ConvertMousePositionToWorldLocation(FVector& WorldLocation);
	

protected:
	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> LineTraceIgnoreActors;
	UPROPERTY(BlueprintReadOnly)
	class AGridManager* GridManagerRef;
	UPROPERTY(BlueprintReadOnly)
	class AStationManager* StationManagerRef;
	UPROPERTY(BlueprintReadOnly)
	class ATrainManager* TrainManagerRef;
	UPROPERTY(BlueprintReadOnly)
	class ALaneManager* LaneManagerRef;
	UPROPERTY(BlueprintReadWrite)
	class ALane* LaneRef;

};
