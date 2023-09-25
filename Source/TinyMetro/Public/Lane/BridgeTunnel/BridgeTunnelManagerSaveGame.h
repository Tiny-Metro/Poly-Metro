// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ConnectorData.h"
#include "BridgeTunnelManagerSaveGame.generated.h"

UCLASS()
class TINYMETRO_API UBridgeTunnelManagerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	TArray<int32> Connectors;

	UPROPERTY()
	int32 Count;
};