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
	if (!IsPointsValid(points)) {
		UE_LOG(LogTemp, Warning, TEXT("The givien pointsArray is invalid"));
		return; 
	}

	FConnectorData* existingConnector = FindConnector(type, points);

	if (existingConnector != nullptr) {
		existingConnector->ConnectorREF->CountUp();
		return;
	}

	TArray<FIntPoint> Points = ProcessArray(points);

	switch (type)
	{
	case ConnectorType::Bridge:
		if (PlayerStateRef->UseBridge()) { 
			CreateNewBridge(Points);
		}
		else { 
			UE_LOG(LogTemp, Warning, TEXT("No Valid Bridge")); 
		}		
		break;

	case ConnectorType::Tunnel:
		if (PlayerStateRef->UseTunnel()) { 
			CreateNewTunnel(Points); 
		}
		else { 
			UE_LOG(LogTemp, Warning, TEXT("No Valid Tunnel")); 
		}
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

TArray<FIntPoint> ABridgeTunnelManager::ProcessArray(const TArray<FIntPoint>& points) {
	TArray<FIntPoint> PointArray;

	PointArray.Add(points[0]);
	for (int i = 1; i < points.Num(); i++)
	{
		if(i == points.Num() -1 ){
			PointArray.Add(points[i]);
			break;
		}

		FIntPoint curr = FIntPoint(points[i].X - points[i-1].X, points[i].Y - points[i-1].Y);
		FIntPoint next = FIntPoint(points[i+1].X - points[i].X, points[i + 1].Y - points[i].Y);
		if (curr != next) {
			PointArray.Add(points[i]);
		}
	}

	return PointArray;
}

bool ABridgeTunnelManager::IsPointsValid(const TArray<FIntPoint>& points) {
	//check if it is empty
	if(points.IsEmpty()) return false;
	// check if the element is only one
	if(points.Num() < 1) return false;
	// check if last = first
	if(points[0]==points.Last()) return false;

	// Check if any points of array is out of range
	FIntPoint GridSize = GridManagerRef->GetGridSize();
	for (int i = 0; i < points.Num(); i++)
	{
		FIntPoint point = points[i];
		if (point.X > GridSize.X || point.X < 0) { return false; }
		if (point.Y > GridSize.Y || point.Y < 0) { return false; }
	}

	return true;
}

FConnectorData* ABridgeTunnelManager::FindConnector(TWeakObjectPtr<ABridgeTunnel> ConnectorREF) {

	for (FConnectorData& connector : Connectors) {
		if (ConnectorREF == connector.ConnectorREF) {
			return &connector;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
	return nullptr;
}
FConnectorData* ABridgeTunnelManager::FindConnector(ConnectorType type, const TArray<FIntPoint> points) {
	TArray<FIntPoint> processedPoints = ProcessArray(points);
	TArray<FIntPoint> reversedPoints = processedPoints;
	Algo::Reverse(reversedPoints);

	UE_LOG(LogTemp, Warning, TEXT("????????"));
	UE_LOG(LogTemp, Warning, TEXT("Number of elements in processedPoints: %d"), processedPoints.Num());
	for (int i = 0; i < processedPoints.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("elements : %d     %d"), processedPoints[i].X, processedPoints[i].Y);

	}


	for (FConnectorData& connector : Connectors) {
		if (type == connector.Type && AreArraysEqual(processedPoints, connector.PointArr) ){ //connector.PointArr == processedPoints) {
			return &connector;
		}
		if (type == connector.Type && AreArraysEqual(reversedPoints, connector.PointArr)){//connector.PointArr == reversedPoints) {
			return &connector;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
	return nullptr;
}
bool ABridgeTunnelManager::AreArraysEqual(const TArray<FIntPoint>& Array1, const TArray<FIntPoint>& Array2)
{
	if (Array1.Num() != Array2.Num())
	{
		return false;
	}

	for (int32 Index = 0; Index < Array1.Num(); Index++)
	{
		if (Array1[Index] != Array2[Index])
		{
			return false;
		}
	}

	return true;
}

void ABridgeTunnelManager::DeleteConnectorByInfo(ConnectorType type, const TArray<FIntPoint>& points) {
	FConnectorData* ConnectorToDelete = FindConnector(type, points);
	if (ConnectorToDelete != nullptr) {
		ConnectorToDelete->ConnectorREF->Destroy();
		Connectors.Remove(*ConnectorToDelete);
	}
}
void ABridgeTunnelManager::DeleteConnectorByActorRef(ABridgeTunnel* ConnectorREF) {
	FConnectorData* ConnectorToDelete = FindConnector(ConnectorREF);
	if (ConnectorToDelete != nullptr) {
		ConnectorToDelete->ConnectorREF->Destroy();
		Connectors.Remove(*ConnectorToDelete);
	}
}
void ABridgeTunnelManager::DeleteConnector(FConnectorData connectorData) {
		connectorData.ConnectorREF->Destroy();
		Connectors.Remove(connectorData);
}
void ABridgeTunnelManager::DisconnectConnector(FConnectorData connectorData) {
	connectorData.ConnectorREF->CountDown();
}

void ABridgeTunnelManager::DisconnectByInfo(ConnectorType type, const TArray<FIntPoint>& points) {
	FConnectorData* ConnectorToDelete = FindConnector(type, points);
	if (ConnectorToDelete == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!The givien pointsArray is invalid!!!!!!!!"));
		return;
	}
	DisconnectConnector(*ConnectorToDelete);
}
void ABridgeTunnelManager::DisconnectByActorRef(ABridgeTunnel* ConnectorREF) {
	FConnectorData* ConnectorToDelete = FindConnector(ConnectorREF);
	DisconnectConnector(*ConnectorToDelete);
}

bool ABridgeTunnelManager::IsConnectorExist(ConnectorType type, const TArray<FIntPoint> points) 
{
	TArray<FIntPoint> processedPoints = ProcessArray(points);
	TArray<FIntPoint> reversedPoints = processedPoints;
	Algo::Reverse(reversedPoints);

	for (FConnectorData& connector : Connectors) {
		if (type == connector.Type && connector.PointArr == processedPoints) {
			return true;
		}
		if (type == connector.Type && connector.PointArr == reversedPoints) {
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
	return false;
}