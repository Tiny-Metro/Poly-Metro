// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PolicyData.h"
#include "PolicySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UPolicySaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FPolicyData PolicyData;
};
