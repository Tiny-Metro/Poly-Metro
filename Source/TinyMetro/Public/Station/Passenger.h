// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StationType.h"
#include "../SaveSystem/TMSaveManager.h"
#include "Passenger.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UPassenger : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetDestination(StationType Dest);
	UFUNCTION()
	StationType GetDestination() const;
	static UPassenger* ConstructPassenger(StationType Dest);


	friend class ATMSaveManager;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	StationType Destination;
	UPROPERTY(BlueprintReadOnly)
	bool IsFree;
	UPROPERTY(BlueprintReadOnly)
	TArray<bool> RandomFree = {true, false, false, false, false};
	
	TQueue<int32>* ShortestRoute;

public:
	UFUNCTION()
	void SetFree();

	void SetPassengerRoute(TQueue<int32>* Route);
	TQueue<int32>* GetPassengerRoute();
	
};
