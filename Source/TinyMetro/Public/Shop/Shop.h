// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShopInfo.h"
#include "../PlayerState/TinyMetroPlayerState.h"
#include "../Station/StationManager.h"
#include "Shop.generated.h"

UCLASS()
class TINYMETRO_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void UpdateShopInfo();

public:
	UFUNCTION(BlueprintCallable)
	FShopInfo GetShopInfo();
	UFUNCTION(BlueprintCallable)
	FShopInfo PurchaseTrain(int32 Amount, bool& Success);
	UFUNCTION(BlueprintCallable)
	FShopInfo PurchaseSubtrain(int32 Amount, bool& Success);
	UFUNCTION(BlueprintCallable)
	FShopInfo PurchaseLane(int32 Amount, bool& Success);
	UFUNCTION(BlueprintCallable)
	FShopInfo PurchaseTunnel(int32 Amount, bool& Success);
	UFUNCTION(BlueprintCallable)
	FShopInfo PurchaseBridge(int32 Amount, bool& Success);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	ATinyMetroPlayerState* PlayerState;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	AStationManager* StationManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class AStatisticsManager* StatisticsManagerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goods")
	FShopInfo ShopInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goods")
	int32 CostTrainIncrease = 50;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Goods")
	int32 CostSubtrainIncrease = 25;
};
