// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConnectorData.h"
#include "GameModes/TinyMetroGameModeBase.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConnectorId ;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CountUp(int32 LaneID);
	virtual void CountUp_Implementation(int32 LaneID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Register();
	virtual void Register_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SendDeathCertificate();
	virtual void SendDeathCertificate_Implementation();


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CountDown(int32 LaneID);
	virtual void CountDown_Implementation(int32 LaneID);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BuildBridgeTunnel();
	virtual void BuildBridgeTunnel_Implementation();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATinyMetroGameModeBase* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATMSaveManager* SaveManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATinyMetroPlayerState* PlayerStateRef;


	UFUNCTION()
	void Save();

	UFUNCTION()
	bool Load();

	UFUNCTION(BlueprintCallable)
	bool IsLanePassing(int32 LaneId); // check the targetLane is passing 
};