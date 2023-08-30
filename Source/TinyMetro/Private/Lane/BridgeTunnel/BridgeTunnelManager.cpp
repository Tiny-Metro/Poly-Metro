// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/BridgeTunnel/BridgeTunnelManager.h"
#include "GridGenerator/GridCellData.h"
#include "SaveSystem/TMSaveManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "GameFramework/Controller.h" //
#include "Lane/BridgeTunnel/BridgeTunnelManagerSaveGame.h"
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
	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	SaveManagerRef = GameMode->GetSaveManager();
	if (SaveManagerRef)
	{
		SaveManagerRef->SaveTask.AddDynamic(this, &ABridgeTunnelManager::Save);
		UE_LOG(LogTemp, Warning, TEXT("SaveManagerRef is Put SaveTask in ABridgeTunnelManager::BeginPlay()"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveManagerRef is not valid in ABridgeTunnelManager::BeginPlay()"));
	}
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
	Count++;
	ABridgeTunnel* existingConnector = FindConnector(type, points);

	if (existingConnector != nullptr) {
		existingConnector->CountUp();
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

ABridgeTunnel* ABridgeTunnelManager::FindConnector(TWeakObjectPtr<ABridgeTunnel> ConnectorREF) {
	if (Connectors.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
		return nullptr;

	}
	for (ABridgeTunnel* connector : Connectors) {
		if (ConnectorREF == connector) {
			return connector;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
	return nullptr;
}
ABridgeTunnel* ABridgeTunnelManager::FindConnector(ConnectorType type, const TArray<FIntPoint> points) {
	if (Connectors.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
		return nullptr;
	}

	TArray<FIntPoint> processedPoints = ProcessArray(points);
	TArray<FIntPoint> reversedPoints = processedPoints;
	Algo::Reverse(reversedPoints);

	UE_LOG(LogTemp, Warning, TEXT("Number of elements in processedPoints: %d"), processedPoints.Num());
	for (int i = 0; i < processedPoints.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("elements : %d     %d"), processedPoints[i].X, processedPoints[i].Y);

	}


	for (ABridgeTunnel* connector : Connectors) {
		FConnectorData targetConnectorInfo = connector->ConnectorInfo;
		if (type == targetConnectorInfo.Type && AreArraysEqual(processedPoints, targetConnectorInfo.PointArr) ){
			return connector;
		}
		if (type == targetConnectorInfo.Type && AreArraysEqual(reversedPoints, targetConnectorInfo.PointArr)){
			return connector;
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
	ABridgeTunnel* ConnectorToDelete = FindConnector(type, points);
	if (ConnectorToDelete != nullptr) {
		ConnectorToDelete->Destroy();
		Connectors.Remove(ConnectorToDelete);
	}
}
void ABridgeTunnelManager::DeleteConnectorByActorRef(ABridgeTunnel* ConnectorREF) {
	ABridgeTunnel* ConnectorToDelete = FindConnector(ConnectorREF);
	if (ConnectorToDelete != nullptr) {
		ConnectorToDelete->Destroy();
		Connectors.Remove(ConnectorToDelete);
	}
}
void ABridgeTunnelManager::DeleteConnector(ABridgeTunnel* Connector) {
		Connector->Destroy();
		Connectors.Remove(Connector);
}
void ABridgeTunnelManager::DisconnectConnector(ABridgeTunnel* Connector) {
	Connector->CountDown();
}

void ABridgeTunnelManager::DisconnectByInfo(ConnectorType type, const TArray<FIntPoint>& points) {
	ABridgeTunnel* ConnectorToDelete = FindConnector(type, points);
	if (ConnectorToDelete == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!The givien pointsArray is invalid!!!!!!!!"));
		return;
	}
	DisconnectConnector(ConnectorToDelete);
}
void ABridgeTunnelManager::DisconnectByActorRef(ABridgeTunnel* ConnectorREF) {
	ABridgeTunnel* ConnectorToDelete = FindConnector(ConnectorREF);
	DisconnectConnector(ConnectorToDelete);
}

bool ABridgeTunnelManager::IsConnectorExist(ConnectorType type, const TArray<FIntPoint> points) 
{
	if (Connectors.IsValidIndex(0))
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no such Connector in the Connectors"));
		return false;
	}
	TArray<FIntPoint> processedPoints = ProcessArray(points);
	TArray<FIntPoint> reversedPoints = processedPoints;
	Algo::Reverse(reversedPoints);

	for (ABridgeTunnel* target : Connectors) {
		FConnectorData connector = target->ConnectorInfo;
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

	void ABridgeTunnelManager::Save()
	{
		UE_LOG(LogTemp, Warning, TEXT("BridgeTunnelManager SaveGame SAVED"));
		UBridgeTunnelManagerSaveGame* tmp = Cast<UBridgeTunnelManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UBridgeTunnelManagerSaveGame::StaticClass()));
		tmp->Count = Count;
		for (auto& i : Connectorss)
		{
			tmp->Connectors.Add(i.Key);
		}

		if (!IsValid(tmp)) 
		{
			UE_LOG(LogTemp, Warning, TEXT("BridgeTunnelManager tmp in Save is not valid"));
		}

		SaveManagerRef->Save(tmp, SaveActorType::BridgeTunnelManager);

	}
	bool ABridgeTunnelManager::Load() 
	{
		UE_LOG(LogTemp, Warning, TEXT("BridgeTunnelManager SaveGame LOADED"));

		if (!GameMode) {
			GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		}
		if (!SaveManagerRef) {
			SaveManagerRef = GameMode->GetSaveManager();
		}	
	
		UBridgeTunnelManagerSaveGame* tmp = Cast<UBridgeTunnelManagerSaveGame>(SaveManagerRef->Load(SaveActorType::BridgeTunnelManager));

		if (!IsValid(tmp)) {
			UE_LOG(LogTemp, Warning, TEXT("BridgeTunnelManager tmp in Load is not valid"));
			return false;
		}

		Count = tmp->Count;
		
		for (auto& i : tmp->Connectors)
		{
			ABridgeTunnel* tmpBridgeTunnel = LoadBridgeTunnel(i);

			Connectorss.Add(i, tmpBridgeTunnel);
		}

		return true;
	}
	ABridgeTunnel* ABridgeTunnelManager::LoadBridgeTunnel(int32 connectorId)
	{
		UE_LOG(LogTemp, Warning, TEXT("BridgeTunnelManager LOAD BridgeTunnel"));
		ABridgeTunnel* tmpBridgeTunnel = SpawnConnector();
		tmpBridgeTunnel->ConnectorId = connectorId;
		tmpBridgeTunnel->Load();
		return tmpBridgeTunnel;
	}
	ABridgeTunnel* ABridgeTunnelManager::SpawnConnector()
	{
		// Load BP Class
		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Lane/BridgeTunnel/BP_BridgeTunnel.BP_BridgeTunnel'")));

		// Cast to BP
		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		// Check object validation
		if (!SpawnActor) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN / BridgeTunnel")));
			return nullptr;
		}

		// Check null
		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
			return nullptr;
		}

		// Spawn actor
		FActorSpawnParameters SpawnParams;
		FTransform SpawnTransform;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABridgeTunnel* tmpBridgeTunnel = Cast<ABridgeTunnel>(GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnParams));

		return tmpBridgeTunnel;
	}