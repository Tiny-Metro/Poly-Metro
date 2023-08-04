// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <UObject/ConstructorHelpers.h>
#include "Finance/Bank.h"
#include "Train/TrainManager.h"
#include "Lane/LaneManager.h"
#include "Policy/Policy.h"
#include "Shop/Shop.h"
#include "Statistics/StatisticsManager.h"
#include "SaveSystem/TMSaveManager.h"
#include "Lane/BridgeTunnel/BridgeTunnelManager.h"
#include "Event/TinyMetroEventManager.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/HUD.h>

ATinyMetroGameModeBase::ATinyMetroGameModeBase() {
    PlayerStateClass = ATinyMetroPlayerState::StaticClass();
    static ConstructorHelpers::FClassFinder<APawn> RTS_Camera(
        TEXT("Blueprint'/Game/Camera/BP_RtsCamera.BP_RtsCamera_C'"));
    if (RTS_Camera.Succeeded()) {
        DefaultPawnClass = RTS_Camera.Class;
    }
    static ConstructorHelpers::FClassFinder<AController> RTS_Controller(
        TEXT("Blueprint'/Game/Camera/BP_RtsController.BP_RtsController_C'"));
    if (RTS_Controller.Succeeded()) {
        PlayerControllerClass = RTS_Controller.Class;
    }
    static ConstructorHelpers::FClassFinder<AHUD> MyHUD(
        TEXT("Blueprint'/Game/Stage/UI/HUD/BP_PlayerHUD.BP_PlayerHUD_C'"));
    if (MyHUD.Succeeded()) {
        HUDClass = MyHUD.Class;
    }
}

FString ATinyMetroGameModeBase::GetFileName() const {
    
    return TEXT("Base");
}

TMap<FIntPoint, StationType> ATinyMetroGameModeBase::GetInitData() const {
    TMap<FIntPoint, StationType> Tmp;
    Tmp.Add(FIntPoint(0, 0), StationType::Circle);
    return Tmp;
}

int32 ATinyMetroGameModeBase::GetDaytime() const {
    return Daytime;
}

TMap<int32, FColor> ATinyMetroGameModeBase::GetLaneColor() const {
    return TMap<int32, FColor>();
}

TArray<FSoftObjectPath> ATinyMetroGameModeBase::GetTrainMaterialPath() const {
    return TrainMaterialPath;
}

TArray<FSoftObjectPath> ATinyMetroGameModeBase::GetLaneMaterialPath() const {
    return LaneMaterialPath;
}

TArray<FSoftObjectPath> ATinyMetroGameModeBase::GetPassengerMaterialPath() const {
    return PassengerMaterialPath;
}

void ATinyMetroGameModeBase::StartPlay() {
    SaveManager = GetWorld()->SpawnActor<ATMSaveManager>();
    StatisticsManager = GetWorld()->SpawnActor<AStatisticsManager>();
    StationManager = GetWorld()->SpawnActor<AStationManager>();
    StationManager->Load();
    LaneManager = GetWorld()->SpawnActor<ALaneManager>();
    LaneManager->Load();
    TrainManager = GetWorld()->SpawnActor<ATrainManager>();
    Policy = GetWorld()->SpawnActor<APolicy>();
    Bank = GetWorld()->SpawnActor<ABank>();
    Timer = GetWorld()->SpawnActor<ATimer>();
    Shop = GetWorld()->SpawnActor<AShop>();
    EventManager = GetWorld()->SpawnActor<ATinyMetroEventManager>();

    // Spawn BP_BridgeTunnelManager
    // Load BP Class
    UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Lane/BridgeTunnel/BP_BridgeTunnelManger.BP_BridgeTunnelManger'")));
    
    // Cast to BP
    UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
    // Check object validation
    if (!SpawnActor) {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
        return;
    }

    // Check null
    UClass* SpawnClass = SpawnActor->StaticClass();
    if (SpawnClass == nullptr) {
        GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
        return;
    }

    // Spawn actor
    FActorSpawnParameters SpawnParams;
    FTransform SpawnTransform;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    BridgeTunnelManager = Cast<ABridgeTunnelManager>(GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, SpawnTransform));

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UE_LOG(LogTemp, Log, TEXT("GameMode::StartPlay : Spawn finish"));
    Super::StartPlay();
}

void ATinyMetroGameModeBase::BeginPlay() {
    Super::BeginPlay();
}

void ATinyMetroGameModeBase::SetGameSpeed(float TimeDilation) {
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
    PlayerController->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
}

AStationManager* ATinyMetroGameModeBase::GetStationManager() const {
    return StationManager;
}

ATMSaveManager* ATinyMetroGameModeBase::GetSaveManager() const {
    return SaveManager;
}

ATrainManager* ATinyMetroGameModeBase::GetTrainManager() const {
    return TrainManager;
}

ALaneManager* ATinyMetroGameModeBase::GetLaneManager() const {
    return LaneManager;
}

APolicy* ATinyMetroGameModeBase::GetPolicy() const {
    return Policy;
}

ABank* ATinyMetroGameModeBase::GetBank() const {
    return Bank;
}

ATimer* ATinyMetroGameModeBase::GetTimer() const {
    return Timer;
}

AShop* ATinyMetroGameModeBase::GetShop() const {
    return Shop;
}

ABridgeTunnelManager* ATinyMetroGameModeBase::GetBridgeTunnelManager() const {
    return BridgeTunnelManager;
}

ATinyMetroEventManager* ATinyMetroGameModeBase::GetEventManager() const {
    return EventManager;
}

AStatisticsManager* ATinyMetroGameModeBase::GetStatisticsManager() const {
    return StatisticsManager;
}
