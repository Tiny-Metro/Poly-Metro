// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PolicyData.h"
#include "Policy.generated.h"


UCLASS()
class TINYMETRO_API APolicy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APolicy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public :
	UPROPERTY()
	FPolicyData PolicyData;


public :
	UPROPERTY()
	TArray<int> ComplainArrayForServiceLevel = { 0,2,1,0,-1,-2 };
	UPROPERTY()
	TArray<int> CostArrayForServiceLevel = { 0,0,1,2,3,4 };
	

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetServiceCostLevel(int costLevel);
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetHandicappedSeat();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetHasCCTV();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetHasElevator( );
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetHasBicycle( );
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetHasTransfer( );

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
		int GetServiceCostLevel();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		bool GetHandicappedSeat();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		bool GetHasCCTV();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		bool GetHasElevator();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		bool GetHasBicycle();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		bool GetHasTransfer();

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
		int GetComplainForServiceLevel();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		int GetCostForServiceLevel();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		int GetCostForCCTV();
	UFUNCTION(BlueprintCallable, Category = "Policy")
		int GetCostForElevator();

	// TODO: HUD 에서 버튼 누르면 바로 반영되게 Set으로 각각 설정

public :
	int32 CalculateComplainPercentage();

public:
	void InitPolicy();
};
