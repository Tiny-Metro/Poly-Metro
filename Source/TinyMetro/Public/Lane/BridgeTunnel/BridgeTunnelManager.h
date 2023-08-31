// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../GridGenerator/GridManager.h"
#include "../../PlayerState/TinyMetroPlayerState.h"
#include "ConnectorType.h"
#include "ConnectorData.h"
#include "BridgeTunnel.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "BridgeTunnelManager.generated.h"

UCLASS()
class TINYMETRO_API ABridgeTunnelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridgeTunnelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ConnectBT(const TArray<FIntPoint>& points);
	virtual void ConnectBT_Implementation(const TArray<FIntPoint>& points);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void DisconnectBT(const TArray<FIntPoint>& points);
	virtual void DisconnectBT_Implementation(const TArray<FIntPoint>& points);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridManager* GridManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ATinyMetroPlayerState* PlayerStateRef;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateNewBridge(const TArray<FIntPoint>& points);
	virtual void CreateNewBridge_Implementation(const TArray<FIntPoint>& points);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void CreateNewTunnel(const TArray<FIntPoint>& points);
	virtual void CreateNewTunnel_Implementation(const TArray<FIntPoint>& points);
	
	UFUNCTION(BlueprintCallable)
	void BuildConnector(ConnectorType type, const TArray<FIntPoint>& points);

	UFUNCTION(BlueprintCallable)
	void ReturnItem(ConnectorType type);

private:
	TArray<FIntPoint> ProcessArray(const TArray<FIntPoint>& points);

	bool IsPointsValid(const TArray<FIntPoint>& points);

public:
	ABridgeTunnel* FindConnector(ConnectorType type, const TArray<FIntPoint> points);
	ABridgeTunnel* FindConnector(TWeakObjectPtr<ABridgeTunnel> ConnectorREF);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ABridgeTunnel*> Connectors;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<int32, ABridgeTunnel*> Connectorss;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;

	UFUNCTION(BlueprintCallable)
	void DeleteConnectorByInfo(ConnectorType type, const TArray<FIntPoint>& points);
	UFUNCTION(BlueprintCallable)
	void DeleteConnectorByActorRef(ABridgeTunnel* ConnectorREF);

	UFUNCTION(BlueprintCallable)
	void DeleteConnector(ABridgeTunnel* Connector);


	UFUNCTION(BlueprintCallable)
	void DisconnectConnector(ABridgeTunnel* Connector);

	UFUNCTION(BlueprintCallable)
	void DisconnectByInfo(ConnectorType type, const TArray<FIntPoint>& points);

	UFUNCTION(BlueprintCallable)
	void DisconnectByActorRef(ABridgeTunnel* ConnectorREF);

public:
	bool IsConnectorExist(ConnectorType type, const TArray<FIntPoint> points);
	bool AreArraysEqual(const TArray<FIntPoint>& Array1, const TArray<FIntPoint>& Array2);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ATinyMetroGameModeBase* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATMSaveManager* SaveManagerRef;

	UFUNCTION()
	void Save();
	UFUNCTION()
	bool Load();

	UFUNCTION()
	ABridgeTunnel* LoadConnector(int32 connectorId);

	ABridgeTunnel* SpawnConnector();

};
