// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../../GridGenerator/GridManager.h"
#include "ConnectorPoint.h"
#include "ConnectorStruct.h"
#include "ConnectorManager.generated.h"

UCLASS(Blueprintable)
class TINYMETRO_API AConnectorManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConnectorManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	AGridManager* GridManagerRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FConnectorPoint> PointArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FConnectorStruct> ConnectorArray;

public:

/*
* 	UFUNCTION(BlueprintCallable)
	void SetConnector();

	UFUNCTION(BlueprintCallable)
	void JoinConnector();

	UFUNCTION(BlueprintCallable)
	void DisjoinConnector();

	UFUNCTION(BlueprintCallable)
	void CreateNewConnector();
s
*/

};
