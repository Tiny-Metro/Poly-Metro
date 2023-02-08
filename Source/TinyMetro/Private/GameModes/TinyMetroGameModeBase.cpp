// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <UObject/ConstructorHelpers.h>
#include "Finance/Bank.h"
#include "Train/TrainManager.h"
#include "Lane/LaneManager.h"
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
    return 12;
}

void ATinyMetroGameModeBase::StartPlay() {
    GetWorld()->SpawnActor<ABank>();
    GetWorld()->SpawnActor<ATrainManager>();
    GetWorld()->SpawnActor<ALaneManager>();
    Super::StartPlay();
}

void ATinyMetroGameModeBase::BeginPlay() {
    Super::BeginPlay();
}

void ATinyMetroGameModeBase::SetGameSpeed(float TimeDilation) {
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
}
