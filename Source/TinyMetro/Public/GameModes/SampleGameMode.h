// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "SampleGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ASampleGameMode : public ATinyMetroGameModeBase
{
	GENERATED_BODY()

public:
	virtual FString GetFileName() const override;
};
