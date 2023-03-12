// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BridgeTunnelManager.generated.h"

UCLASS()
class TINYMETRO_API ABridgeTunnelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridgeTunnelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ConnectBT(const TArray<FIntPoint>& points);
	virtual void ConnectBT_Implementation(const TArray<FIntPoint>& points);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisconnectBT(const TArray<FIntPoint>& points);
	virtual void DisconnectBT_Implementation(const TArray<FIntPoint>& points);

};
