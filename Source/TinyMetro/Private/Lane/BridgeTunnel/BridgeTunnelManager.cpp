// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/BridgeTunnel/BridgeTunnelManager.h"
#include "GridGenerator/GridCellData.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABridgeTunnelManager::ABridgeTunnelManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABridgeTunnelManager::BeginPlay()
{
	Super::BeginPlay();
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
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


void ABridgeTunnelManager::BuildConnector(ConnectorType type, const TArray<FIntPoint>& points) {
	switch (type)
	{
	case ConnectorType::Bridge:
		if (PlayerStateRef->UseBridge()) {
			CreateNewBridge(points);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("No Valid Bridge")); }
		
		break;

	case ConnectorType::Tunnel:
		if (PlayerStateRef->UseTunnel()) {
			CreateNewTunnel(points);
		}
		else { UE_LOG(LogTemp, Warning, TEXT("No Valid Tunnel")); }
		break;
	default:
		break;
	}

}
void ABridgeTunnelManager::ReturnItem(ConnectorType type) {
	
	switch (type) {
	case ConnectorType::Bridge:
		PlayerStateRef->AddItem(ItemType::Bridge, 1);
		break;
	case ConnectorType::Tunnel:
		PlayerStateRef->AddItem(ItemType::Tunnel, 1);
		break;
	default:
		break;
	}
}
*/