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
	SaveManagerRef = GameMode->GetSaveManager();
}

// Called every frameD
void ABridgeTunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ABridgeTunnel::CountUp_Implementation() {};
void ABridgeTunnel::CountDown_Implementation() {};
void ABridgeTunnel::BuildBridgeTunnel_Implementation() {};

void ABridgeTunnel::BuildTest()
{
	BuildBridgeTunnel();
}
void ABridgeTunnel::Save()
{
	UBridgeTunnelSaveGame* tmp = Cast<UBridgeTunnelSaveGame>(UGameplayStatics::CreateSaveGameObject(UBridgeTunnelSaveGame::StaticClass()));
	tmp->ConnectorId = ConnectorId;
	tmp->count = count;

	FConnectorData ConnectorData;
	ConnectorData.PointArr = ConnectorInfo.PointArr;
	ConnectorData.Type = ConnectorInfo.Type;
	tmp->ConnectorInfo = ConnectorData;


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

	UE_LOG(LogTemp, Warning, TEXT("Lane %d is loaded"), ConnectorId);
	
	ConnectorId = tmp ->ConnectorId;
	ConnectorInfo = tmp ->ConnectorInfo;
	count = tmp -> count;

	BuildBridgeTunnel();
	return true;
}