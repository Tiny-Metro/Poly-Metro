// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvestmentStartData.h"
#include "InvestmentManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRefreshTask);

UCLASS()
class TINYMETRO_API AInvestmentManager : public AActor
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Sets default values for this component's properties
	AInvestmentManager();

	UFUNCTION()
	void InitInvestment();
	UFUNCTION()
	void NotifyInvestmentStart(int32 Id);
	UFUNCTION()
	void NotifyInvestmentFinish(int32 Id);
	UFUNCTION(BlueprintCallable)
	class UInvestment* GetInvestmentById(int32 Id);
	UFUNCTION(BlueprintCallable)
	TMap<int32, class UInvestment*> GetAllInvestment();

	// Update candidate
	UFUNCTION(BlueprintCallable)
	void RefreshAccessibleInvestment();
	UFUNCTION()
	void UpdateAccessibleInvestment();

	// Weekly task (call by timer)
	UFUNCTION()
	void WeeklyTask();

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

	// Call by InvestmentLuaState
	UFUNCTION()
	FInvestmentStartData GetInvestmentStartData(int32 Id);

	UPROPERTY(BlueprintAssignable)
	FRefreshTask RefreshTask;

protected:
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY()
	class ATimer* TimerRef;
	UPROPERTY()
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY()
	class AStationManager* StationManagerRef;
	UPROPERTY()
	class ATrainManager* TrainManagerRef;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	class UInvestmentLuaState* LuaState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TMap<int32, class UInvestment*> LuaComponentArr;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TArray<int32> InvestmentIdArr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TMap<int32, FInvestmentStartData> InvestmentStartData;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool CanRefresh = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 MaxInvestmentSlot = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 InvestmentSlot = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TMap<int32, int32> InvestmentCandidate;

	UPROPERTY()
	bool IsLoaded = false;
};
