// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StationType.h"
#include "Passenger.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UPassenger : public UObject
{
	GENERATED_BODY()

public:
	void SetDestination(StationType Dest);
	static UPassenger* ConstructPassenger(StationType Dest);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	StationType Destination;
	
};
