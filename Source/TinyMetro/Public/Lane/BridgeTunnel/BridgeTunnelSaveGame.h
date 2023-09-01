// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ConnectorData.h"
#include "BridgeTunnelSaveGame.generated.h"

UCLASS()
class TINYMETRO_API UBridgeTunnelSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	FConnectorData ConnectorInfo;

	UPROPERTY()
	int32 count;

	UPROPERTY()
	int32 ConnectorId;
};