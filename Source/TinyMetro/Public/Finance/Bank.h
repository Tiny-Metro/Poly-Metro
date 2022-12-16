// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Sample/SampleTimer.h"
#include "Loan.h"
#include "Bank.generated.h"

UCLASS()
class TINYMETRO_API ABank : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABank();

public:
	UFUNCTION(BlueprintCallable)
	TArray<ULoan*> GetAllLoan() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ULoan* CreateLoan(FLoanData Data);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	TArray<ULoan*> Loan;
};
