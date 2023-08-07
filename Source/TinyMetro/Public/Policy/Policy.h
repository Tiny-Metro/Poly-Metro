// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PolicyData.h"
#include "ServiceData.h"
#include "Policy.generated.h"


class AStationManager;

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

protected :
	UPROPERTY(VisibleAnywhere, Category = "Policy data")
	FPolicyData PolicyData;

	UPROPERTY()
	AStationManager* StationManagerRef;

public :
	UPROPERTY(VisibleAnywhere, Category = "Policy data")
	TArray<FServiceData> ServiceLevel = {
		FServiceData(),
		FServiceData(1, 2, 0),
		FServiceData(2, 1, 1),
		FServiceData(3, 0, 2),
		FServiceData(4, -1, 3),
		FServiceData(5, -2, 4)
	};
	

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
	void SetServiceCostLevel(int costLevel);
	UFUNCTION(BlueprintCallable, Category = "Policy")
	void SetPrioritySeat(bool Flag);
	UFUNCTION(BlueprintCallable, Category = "Policy")
	void SetCCTV(bool Flag);
	UFUNCTION(BlueprintCallable, Category = "Policy")
	void SetElevator(bool Flag);
	UFUNCTION(BlueprintCallable, Category = "Policy")
	void SetBicycle(bool Flag);
	UFUNCTION(BlueprintCallable, Category = "Policy")
	void SetTransfer(bool Flag);

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
	FPolicyData GetPolicyData() const;
	UFUNCTION(BlueprintCallable, Category = "Policy")
	int GetServiceCostLevel();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	bool GetPrioritySeat();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	bool GetCCTV();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	bool GetElevator();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	bool GetBicycle();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	bool GetTransfer();

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
	int GetComplainForServiceLevel();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	int GetCostForServiceLevel();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	int GetCostForCCTV();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	int GetCostForElevator();
	UFUNCTION(BlueprintCallable, Category = "Policy")
	int GetTotalCost();

	// TODO: HUD 에서 버튼 누르면 바로 반영되게 Set으로 각각 설정

public :
	int32 CalculateComplainPercentage();

public:
	void InitPolicy();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Policy data")
	float TotalComplainIncrease = 0.0f;
	UPROPERTY(VisibleAnywhere, Category = "Policy data")
	int32 TotalCost = 0.0f;

};
