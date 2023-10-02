// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InvestmentManager.generated.h"


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

	// Daily, Weekly task (call by timer)
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
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY()
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY()
	class ATimer* TimerRef;
	UPROPERTY(VisibleAnywhere, Category = "Info")
	class UInvestmentLuaState* LuaState;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	TMap<int32, class UInvestment*> LuaComponentArr;

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
