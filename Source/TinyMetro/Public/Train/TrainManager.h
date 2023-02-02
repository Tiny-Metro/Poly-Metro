// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrainTemplate.h"
#include "TrainType.h"
#include "TrainManager.generated.h"

UCLASS()
class TINYMETRO_API ATrainManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void AddTrain(ATrainTemplate* Train);
	UFUNCTION(BlueprintCallable)
	void SetTrainId(ATrainTemplate* Train);
	UFUNCTION(BlueprintCallable)
	ATrainTemplate* GetTrainById(int32 TrainId, TrainType& Type);
	UFUNCTION(BlueprintCallable)
	FVector GetNearestTrainLocation(FVector CurrentLocation);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	TArray<ATrainTemplate*> Trains;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	int32 NextTrainId;

};
