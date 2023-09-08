// Fill out your copyright notice in the Description page of Project Settings.
#include "Lane/BridgeTunnel/BridgeTunnel.h"
#include "SaveSystem/TMSaveManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/Controller.h" //
#include "Lane/BridgeTunnel/BridgeTunnelSaveGame.h"
#include "PlayerState/TinyMetroPlayerState.h"//

// Sets default values
ABridgeTunnel::ABridgeTunnel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABridgeTunnel::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	SaveManagerRef = GameMode->GetSaveManager();
	if (SaveManagerRef)
	{
		SaveManagerRef->SaveTask.AddDynamic(this, &ABridgeTunnel::Save);
		UE_LOG(LogTemp, Warning, TEXT("SaveManagerRef is Put SaveTask in ABridgeTunnel::BeginPlay()"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveManagerRef is not valid in ABridgeTunnel::BeginPlay()"));
	}
}

// Called every frameD
void ABridgeTunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABridgeTunnel::CountUp_Implementation(int32 LaneID) {};
void ABridgeTunnel::CountDown_Implementation(int32 LaneID) {};
void ABridgeTunnel::BuildBridgeTunnel_Implementation() {};

void ABridgeTunnel::Register_Implementation() {};
void ABridgeTunnel::SendDeathCertificate_Implementation() {};

void ABridgeTunnel::Save()
{
	UE_LOG(LogTemp, Warning, TEXT("BridgeTunnel %d Saved Called "), ConnectorId);

	UBridgeTunnelSaveGame* tmp = Cast<UBridgeTunnelSaveGame>(UGameplayStatics::CreateSaveGameObject(UBridgeTunnelSaveGame::StaticClass()));
	tmp->ConnectorId = ConnectorId;
	tmp->count = count;

	FConnectorData ConnectorData;
	for (const auto& i : ConnectorInfo.PointArr)
	{
		ConnectorData.PointArr.Add(i);
	}
	ConnectorData.Type = ConnectorInfo.Type;
	for (const auto& i : ConnectorInfo.LaneUseCount)
	{
		ConnectorData.LaneUseCount.Add(i);
	}
	tmp->ConnectorInfo = ConnectorData;
	if (!IsValid(tmp))
	{
		UE_LOG(LogTemp, Warning, TEXT("BridgeTunnel tmp in Save is not valid"));
	}

	SaveManagerRef->Save(tmp, SaveActorType::BridgeTunnel, ConnectorId);
}
bool ABridgeTunnel::Load()
{
	if (!GameMode) {
		GameMode = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}
	if (!SaveManagerRef) {
		SaveManagerRef = GameMode->GetSaveManager();
	}
	UBridgeTunnelSaveGame* tmp = Cast<UBridgeTunnelSaveGame>(SaveManagerRef->Load(SaveActorType::BridgeTunnel, ConnectorId));

	if (!IsValid(tmp)) {
		UE_LOG(LogTemp, Warning, TEXT("BridgeTunnel %d SaveGame is not valid"), ConnectorId);
		return false;
	}

	UE_LOG(LogTemp, Warning, TEXT("BridgeTunnel  %d is loaded"), ConnectorId);
	
	ConnectorId = tmp ->ConnectorId;
	for (const auto& i : tmp->ConnectorInfo.PointArr)
	{
		ConnectorInfo.PointArr.Add(i);
	}
	for (const auto& i : tmp->ConnectorInfo.LaneUseCount)
	{
		ConnectorInfo.LaneUseCount.Add(i);
	}
	ConnectorInfo.Type = tmp->ConnectorInfo.Type;
	count = tmp -> count;

	BuildBridgeTunnel();
	return true;
}
bool ABridgeTunnel::IsLanePassing(int32 LaneId)
{
	// Check if the lane exists in the LaneUseCount map
	int32* LaneCountPtr = ConnectorInfo.LaneUseCount.Find(LaneId);

	// If the lane does not exist or its count is not greater than 0, return false
	if (LaneCountPtr == nullptr || *LaneCountPtr <= 0)
	{
		return false;
	}

	// If the count is greater than 0, return true
	return true;
}