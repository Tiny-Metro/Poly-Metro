// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <UObject/ConstructorHelpers.h>
#include "Finance/Bank.h"
#include "Train/TrainManager.h"
#include "Lane/LaneManager.h"
#include "Policy/Policy.h"
#include "Shop/Shop.h"
#include "SaveSystem/TMSaveManager.h"
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
    StationManager = GetWorld()->SpawnActor<AStationManager>();
    SaveManager = GetWorld()->SpawnActor<ATMSaveManager>();
    TrainManager = GetWorld()->SpawnActor<ATrainManager>();
    LaneManager = GetWorld()->SpawnActor<ALaneManager>();
    Policy = GetWorld()->SpawnActor<APolicy>();
    Bank = GetWorld()->SpawnActor<ABank>();
    Timer = GetWorld()->SpawnActor<ATimer>();
    Shop = GetWorld()->SpawnActor<AShop>();
    UE_LOG(LogTemp, Log, TEXT("GameMode::StartPlay : Spawn finish"));
    Super::StartPlay();
}

void ATinyMetroGameModeBase::BeginPlay() {
    Super::BeginPlay();
}

void ATinyMetroGameModeBase::SetGameSpeed(float TimeDilation) {
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
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
