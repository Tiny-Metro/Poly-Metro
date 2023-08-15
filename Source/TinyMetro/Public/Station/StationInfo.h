// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StationType.h"
#include "UObject/NoExportTypes.h"
#include "StationInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FStationInfo
{
public:
	GENERATED_BODY()


	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Id;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	StationType Type;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 ServiceLaneCount = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalUsingPassenger = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyUsingPassenger = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalProfit = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyProfit = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float Complain = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 GetOnPassengerCount = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float TotalWaitTime = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float AverageWaitTime = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalTransferPassenger = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyTransferPassenger = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsActive = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsDestroyed = false;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsUpgrade = false;


	FStationInfo() { Id = 0; Type = StationType::Circle; }

	FStationInfo(int32 a, StationType _Type) {
		Id = a;
		Type = _Type;
	}

	bool operator==(const FStationInfo& Other) const
	{
		return Id == Other.Id && Type == Other.Type;
	}


	friend uint32 GetTypeHash(const FStationInfo& StaitonInfo);
};
