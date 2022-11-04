// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCellData.h"
#include "GridManager.generated.h"

UCLASS()
class TINYMETRO_API AGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetGridCellSize() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void SetGridCellData(TArray<FGridCellData>& data);
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FGridCellData> GetGridCellData() const;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	float GridCellSize = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	TArray<FGridCellData> GridCellData;
};
