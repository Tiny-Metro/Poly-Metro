// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeSample.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API AGameModeSample : public AGameModeBase
{
	GENERATED_BODY()

public:
	FString GetFileName() const;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	FString MapFileName = "map.txt";
};
