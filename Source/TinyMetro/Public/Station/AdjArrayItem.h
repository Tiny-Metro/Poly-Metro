// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StationInfo.h"
#include "AdjArrayItem.generated.h"
/**
 * 
 */
UCLASS()
class TINYMETRO_API UAdjArrayItem : public UObject
{
	GENERATED_BODY()

public :
	float operator[] (FStationInfo item) { return length[item]; }

	void Add(FStationInfo Item, float Length);

	void RemoveRef(FStationInfo Item);

	int32 Num();

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<FStationInfo, float> length;


};
