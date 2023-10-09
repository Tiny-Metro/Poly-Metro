// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loan.h"
#include "Investment.h"
#include "Bank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInvestmentUpdateTask);

UCLASS()
class TINYMETRO_API ABank : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABank();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Function loan
	UFUNCTION(BlueprintCallable)
	TArray<ULoan*> GetAllLoan() const;
	ULoan* CreateLoan(FLoanData Data, TFunction<bool(void)> Func);
	UFUNCTION()
	void InitLoan();

	// Broadcast by Timer
	UFUNCTION(BlueprintCallable)
	void DailyTask();
	UFUNCTION(BlueprintCallable)
	void WeeklyTask();

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	bool Load();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY(VisibleAnywhere)
	class ATimer* TimerRef;
	UPROPERTY(VisibleAnywhere)
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY(VisibleAnywhere)
	class AStatisticsManager* StatisticsManagerRef;

	UPROPERTY(VisibleAnywhere)
	bool IsLoadSuccess = false;

protected:
	// For loan
	UPROPERTY(BlueprintReadOnly)
	TArray<ULoan*> Loan;

};
