// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdjArrayItem.h"
#include "AdjItem.h"
#include "AdjList.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UAdjList : public UObject
{
	GENERATED_BODY()

public:
	UAdjArrayItem& operator[] (FAdjItem item) { return AdjList[item]; }


	TMap<FAdjItem, UAdjArrayItem> AdjList;
	
};
