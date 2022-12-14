// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class TINYMETRO_API SampleTimer
{
public:
	SampleTimer(UWorld* World, int32 Amount, int32 DueDate, float Rate);
	~SampleTimer();

public:
	//static SampleTimer* ConstructSample(int32 Amount, int32 DueDate, float Rate);
	void Test();

protected:
	int32 Amount;
	int32 DueDate;
	int32 Interest;
	float Rate;
	FTimerHandle TestHandle;
	UWorld* World;
};
