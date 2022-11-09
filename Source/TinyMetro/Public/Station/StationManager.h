// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StationManager.generated.h"

UCLASS()
class TINYMETRO_API AStationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void TestFunction();

	UFUNCTION(BlueprintCallable)
	void SpawnStation();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float StationSpawnRequire = 10000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float StationSpawnPerSec = 1000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float StationSpawnCurrent = 0.0f;
};
