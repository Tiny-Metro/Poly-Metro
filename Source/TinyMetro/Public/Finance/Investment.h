// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LuaComponent.h"
#include "UObject/NoExportTypes.h"
#include "InvestmentData.h"
#include "InvestmentState.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "Investment.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TINYMETRO_API UInvestment : public ULuaComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UInvestment();
	
	UFUNCTION()
	void InitInvestment();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Start();
	UFUNCTION()
	void Success();
	UFUNCTION()
	void Fail();

	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

	UFUNCTION()
	void DailyTask();

	UFUNCTION()
	bool GetAppearance();
	UFUNCTION()
	InvestmentState GetState();

protected:
	// Class references
	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef = nullptr;
	UPROPERTY()
	class AInvestmentManager* InvestmentManagerRef = nullptr;
	UPROPERTY()
	class ATMSaveManager* SaveManagerRef = nullptr;
	UPROPERTY()
	class ATimer* TimerRef = nullptr;
	
	// Default data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 InvestmentId = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	FString Message = TEXT("");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 TimeRequire = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	FString AwardMessage = TEXT("");
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	bool IsStart = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	InvestmentState State = InvestmentState::Ready;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 RemainTime = 0;

	// Mutex
	TSharedPtr<FCriticalSection> MutexKey = MakeShared<FCriticalSection>();
};
