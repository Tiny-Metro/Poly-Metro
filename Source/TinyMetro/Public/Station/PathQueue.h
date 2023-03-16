// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
/**
 * 
 */
class TINYMETRO_API PathQueue
{

public:

	void Enqueue(int32 Data);
	int32 Dequeue();
	int32 Peek();
	int32 Num() const;
	bool IsEmpty() const;

	TArray<int32> TestGetPath();

protected:
	TArray<int32> Path;
};
