// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdjArrayItem.h"
#include "StationInfo.h"
#include "AdjList.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UAdjList : public UObject
{
	GENERATED_BODY()

public:
	UAdjArrayItem& operator[] (FStationInfo item) 
	{ 
		for (auto& i : AdjList) {
			if (item == i.Key) return *i.Value;
		}
		return *AdjList[item]; 
	}

	void Add(FStationInfo Item, UAdjArrayItem* ArrayItem);


protected:
	UPROPERTY(VisibleAnywhere)
	TMap<FStationInfo, UAdjArrayItem*> AdjList;
	
};
