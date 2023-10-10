// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyMetroEventOccurData.h"
#include "TinyMetroEventData.h"
#include "TinyMetroEventManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventOccurTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventEndTask);

UCLASS()
class TINYMETRO_API ATinyMetroEventManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	ATinyMetroEventManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Initializer
	UFUNCTION()
	void InitializeEvent();

	// Event
	UFUNCTION()
	void SimulateProbability();
	UFUNCTION()
	void OccurEvent();
	UFUNCTION()
	void UpdateEventWeight();
	UFUNCTION()
	FTinyMetroEventData GetEventData(int32 EventId);

	// Occur specific event
	// Not used in game (Only test)
	UFUNCTION()
	bool OccurSpecificEvent(int32 EventId);

	// Broadcast by Timer
	UFUNCTION()
	void DailyTask();

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

	UPROPERTY(BlueprintAssignable)
	FEventOccurTask EventOccurTask;
	UPROPERTY(BlueprintAssignable)
	FEventEndTask EventEndTask;

protected:
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY()
	bool IsLoaded = false;
	UPROPERTY()
	class ATimer* TimerRef;
	UPROPERTY()
	class ASoundManager* SoundManagerRef;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	class UEventLuaState* LuaState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Event")
	TMap<int32, class ULuaComponent*> LuaComponentArr;
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Event")
	//TMap<int32, FTinyMetroEventData> EventDataArr;

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
	uint32 ProgressDay = 0;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 ExpirationDay = 0;

	UPROPERTY(VisibleAnywhere, Category = "Event")
	TArray<int32> EventCandidate;
	//UPROPERTY(BlueprintReadOnly, Category = "Event")
	//class UTinyMetroEvent* SelectedEvent;
	//UPROPERTY(VisibleAnywhere, Category = "Event")
	//TMap<int32, class UTinyMetroEvent*> EventArr;

	UPROPERTY(BlueprintReadOnly, Category = "Event")
	TArray<FTinyMetroEventOccurData> EventOccurLog;
};
