// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConnectorData.h"
#include "BridgeTunnel.generated.h"

UCLASS()
class TINYMETRO_API ABridgeTunnel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridgeTunnel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FConnectorData ConnectorInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 count;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CountUp();
	virtual void CountUp_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CountDown();
	virtual void CountDown_Implementation();

};