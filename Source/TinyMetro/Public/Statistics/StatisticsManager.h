// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DefaultStatistics.h"
#include "LaneStatistics.h"
#include "ShopStatistics.h"
#include "BankStatistics.h"
#include "StatisticsManager.generated.h"

UCLASS()
class TINYMETRO_API AStatisticsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStatisticsManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Daily, Weekly task
	UFUNCTION()
	void DailyTask();
	UFUNCTION()
	void WeeklyTask();

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

protected:
	// Other actors reference
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	class ATimer* TimerRef = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	class ATinyMetroGameModeBase* GameModeRef = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reference")
	class ATMSaveManager* SaveManagerRef = nullptr;

	// Statistics structs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	FDefaultStatistics DefaultStatistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	FLaneStatistics LaneStatistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	FShopStatistics ShopStatistics;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statistics")
	FBankStatistics BankStatistics;
};
