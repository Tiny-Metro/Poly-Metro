// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StationType.h"
#include "UObject/NoExportTypes.h"
#include "AdjItem.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FAdjItem
{
	GENERATED_BODY()


	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	int32 Id;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	StationType Type;

	FAdjItem() { Id = 0; Type = StationType::Circle; }
	
	FAdjItem(int32 a, StationType _Type) {
		Id = a;
		Type = _Type;
	}

	bool operator==(const FAdjItem& Other) const
	{
		return Id == Other.Id && Type == Other.Type;
	}

	FORCEINLINE uint32 GetTypeHash(const FAdjItem& AdjItem);

	/*
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	float Length;
	*/
};
