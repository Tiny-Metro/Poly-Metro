// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Timestamp.h"
#include "Timer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDailyTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeeklyTask);

UCLASS()
class TINYMETRO_API ATimer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SkipTime(int32 Skipday = 1);
	UFUNCTION(BlueprintCallable)
	FTimestamp GetTimestamp();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float ElapseTimeSec;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	FTimestamp Timestamp;
	UPROPERTY(BlueprintAssignable)
	FDailyTask DailyTask;
	UPROPERTY(BlueprintAssignable)
	FWeeklyTask WeeklyTask;

protected:
	UPROPERTY()
	class AStationManager* StationManagerRef;
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	int32 Daytime;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	float DayCounter = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	float WeekCounter = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Develop")
	float SkiptimeTarget = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Develop")
	bool SkiptimeFlag = false;
};
