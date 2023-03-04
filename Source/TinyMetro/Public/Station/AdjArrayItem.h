// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdjItem.h"
#include "AdjArrayItem.generated.h"
/**
 * 
 */
UCLASS()
class TINYMETRO_API UAdjArrayItem : public UObject
{
	GENERATED_BODY()

public :
	float operator[] (FAdjItem item) { return length[item]; }

	

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TMap<FAdjItem, float> length;

	/*
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	TArray<FAdjItem> AdjItems;
	*/

};
