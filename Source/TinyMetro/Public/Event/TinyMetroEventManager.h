// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyMetroEventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventOccurTask);

UCLASS()
class TINYMETRO_API ATinyMetroEventManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATinyMetroEventManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void DailyTask();
	UFUNCTION()
	void SimulateProbability();
	UFUNCTION()
	void OccurEvent();
	UFUNCTION()
	void UpdateEventWeight();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FEventOccurTask EventOccurTask;

protected:
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	TArray<float> EventOccurProbability = {
		0,0,0,0,0,0,0,0,0,0,0,0,0,0,
		0.3333, // Day 15
		0.3667,
		0.4000,
		0.4333,
		0.4667,
		0.5000, // Day 20
		0.5333,
		0.5667,
		0.6000,
		0.6333,
		0.6667, // Day 25
		0.7000,
		0.7333,
		0.7667,
		0.8000,
		0.8333, // Day 30
		0.8667,
		0.9000,
		0.9333,
		0.9667,
		0 // Day 35
	};
	UPROPERTY(VisibleAnywhere, Category = "Info")
	uint32 Day = 0;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 EventPeriod = 0;
};
