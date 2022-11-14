// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridCellData.h"
#include "GridBuilder.generated.h"

class AGridManager;

UCLASS()
class TINYMETRO_API AGridBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGridBuilder();

private:
	FVector CalcStartLocation(float gridCellSize) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FGridCellData> GetGridPoints() const;
	UFUNCTION(BlueprintCallable)
	void LoadMapData();
	UFUNCTION(BlueprintCallable)
	TArray<FGridCellData> CalculateGridData(float GridCellSize);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	FString MapFileName = TEXT("map.txt");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	TArray<int32> LoadedGridType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config")
	FIntPoint GridSize;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	AGridManager* GridManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeAllocation")
	int32 GeneratorIndex;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RuntimeAllocation")
	TArray<FGridCellData> GridPoints;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* defaultRoot = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* meshGround = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* meshWater = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* meshHill = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test")
	int32 ChildCount = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Test")
	FVector StartLocation;
};
