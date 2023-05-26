// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loan.h"
#include "Investment.h"
#include "Bank.generated.h"

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
	// UFunction loan
	UFUNCTION(BlueprintCallable)
	TArray<ULoan*> GetAllLoan() const;
public:
	// UFunction investment
	UFUNCTION(BlueprintCallable)
	int32 GetInvestmentStock() const;
	UFUNCTION(BlueprintCallable)
	TArray<UInvestment*> GetAllInvestment() const;
	UFUNCTION(BlueprintCallable)
	TArray<UInvestment*> GetAvailableInvestment() const;
	UFUNCTION(BlueprintCallable)
	TArray<UInvestment*> RefreshInvestment();
	UFUNCTION(BlueprintCallable)
	void UpdateInvestment();
	UFUNCTION(BlueprintCallable)
	void DailyTask();


protected:
	// Function loan
	ULoan* CreateLoan(FLoanData Data, TFunction<bool(void)> Func);
	void InitLoan();
protected:
	// Function investment
	void InitInvestment();
	//UInvestment* CreateInvestment(FInvestmentData Data, TFunction<InvestmentResult(void)> Func);
	void ChangeInvestment(int Index);
	void ChangeAllInvestment();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	int32 Daytime;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(BlueprintReadOnly)
	ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY(VisibleAnywhere)
	class UInvestmentLuaState* LuaState;

protected:
	// For loan
	UPROPERTY(BlueprintReadOnly)
	TArray<ULoan*> Loan;
	//UPROPERTY(BlueprintReadOnly)
	//ATinyMetroGameModeBase* GameMode;

protected:
	// For investment
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Investment")
	int32 InvestmentStock = 1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Investment")
	TArray<UInvestment*> Investment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Investment")
	TArray<int32> AvailInvestment;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Investment")
	int32 MaxInvestmetStock = 3;
	FTimerHandle InvestmentUpdateHandle;

};
