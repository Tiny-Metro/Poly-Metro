// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "../Station/StationType.h"
#include "TinyMetroGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ATinyMetroGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual FString GetFileName() const;
	virtual TMap<FIntPoint, StationType> GetInitData() const;
};
