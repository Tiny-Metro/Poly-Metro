// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StationType.h"
#include "Passenger.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FPassenger
{
	GENERATED_USTRUCT_BODY()

public:
	/*UFUNCTION()
	void SetDestination(StationType Dest);
	UFUNCTION()
	StationType GetDestination() const;
	static FPassenger* ConstructPassenger(StationType Dest);*/

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	StationType Destination;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool IsFree;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<bool> RandomFree = {true, false, false, false, false};
	
	//PathQueue ShortestPath;

public:
	/*UFUNCTION()
	void SetFree();
	UFUNCTION()
	bool GetFree() const;*/

	//void SetPassengerPath(PathQueue Path);
	//PathQueue GetPassengerPath();
	
};
