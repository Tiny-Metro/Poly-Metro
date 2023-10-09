// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FPlayerItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Lane = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Train = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Subtrain = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Tunnel = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Bridge = 0;
};
