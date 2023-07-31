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
	int32 ServiceLaneCount;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalUsingPassenger;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyUsingPassenger;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalProfit;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyProfit;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float Complain;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float AverageWaitTime;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 TotalTransferPassenger;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 WeeklyTransferPassenger;


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
