// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StationType.h"
#include "Station.generated.h"

UCLASS()
class TINYMETRO_API AStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetStationId(int32 Id);
	UFUNCTION(BlueprintCallable)
	void SetStationType(StationType Type);
	UFUNCTION(BlueprintCallable)
	void CalculateComplain();
	UFUNCTION(BlueprintCallable)
	void ActivateStation();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 ComplainMax = 10000;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 ComplainPerSec = 1000;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Variable")
	int32 ComplainCurrent = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 StationId = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	bool IsActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	StationType StationTypeValue = StationType::Circle;
};
