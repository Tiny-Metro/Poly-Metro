// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/Connector/ConnectorManager.h"

// Sets default values
AConnectorManager::AConnectorManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConnectorManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConnectorManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

