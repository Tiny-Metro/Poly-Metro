// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/BridgeTunnel/BridgeTunnelManager.h"
#include "GridGenerator/GridCellData.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABridgeTunnelManager::ABridgeTunnelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
}

// Called when the game starts or when spawned
void ABridgeTunnelManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABridgeTunnelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABridgeTunnelManager::ConnectBT_Implementation(const TArray<FIntPoint>& points) {}
void ABridgeTunnelManager::DisconnectBT_Implementation(const TArray<FIntPoint>& points) {}

void ABridgeTunnelManager::CreateNewBridge_Implementation(const TArray<FIntPoint>& points) {}
void ABridgeTunnelManager::CreateNewTunnel_Implementation(const TArray<FIntPoint>& points) {}