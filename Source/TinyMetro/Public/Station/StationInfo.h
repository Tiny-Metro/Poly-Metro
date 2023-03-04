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
