// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <UObject/ConstructorHelpers.h>
#include "Finance/Bank.h"
#include "Finance/InvestmentManager.h"
#include "Train/TrainManager.h"
#include "Lane/LaneManager.h"
#include "Policy/Policy.h"
#include "Shop/Shop.h"
#include "Statistics/StatisticsManager.h"
#include "Sound/SoundManager.h"
#include "SaveSystem/TMSaveManager.h"
#include "Lane/BridgeTunnel/BridgeTunnelManager.h"
#include "Event/TinyMetroEventManager.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/HUD.h>

// Test include
#include "Lua/InvestmentLuaState.h"
#include "Lua/EventLuaState.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"

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

    ObjBridgeTunnelManager = ConstructorHelpers::FObjectFinder<UClass>(TEXT("Class'/Game/Lane/BridgeTunnel/BP_BridgeTunnelManger.BP_BridgeTunnelManger_C'")).Object;
}

FString ATinyMetroGameModeBase::GetFileName() const {
    
    return TEXT("Base");
}

FString ATinyMetroGameModeBase::GetMapName() const {
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
    TrainManager->Load();
    Policy = GetWorld()->SpawnActor<APolicy>();
    Policy->Load();
    Bank = GetWorld()->SpawnActor<ABank>();
    Timer = GetWorld()->SpawnActor<ATimer>();
    Shop = GetWorld()->SpawnActor<AShop>();
    EventManager = GetWorld()->SpawnActor<ATinyMetroEventManager>();
    EventManager->Load();
    InvestmentManagerRef = GetWorld()->SpawnActor<AInvestmentManager>();
    SoundManagerRef = GetWorld()->SpawnActor<ASoundManager>();

    // Spawn actor
    FActorSpawnParameters SpawnParams;
    FTransform SpawnTransform;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    BridgeTunnelManager = Cast<ABridgeTunnelManager>(GetWorld()->SpawnActor<AActor>(ObjBridgeTunnelManager, SpawnTransform));
    BridgeTunnelManager->Load();

    PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UE_LOG(LogTemp, Log, TEXT("GameMode::StartPlay : Spawn finish"));
    Super::StartPlay();
}

void ATinyMetroGameModeBase::BeginPlay() {
    Super::BeginPlay();
    GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("GameMode::BeginPlay"));
}

void ATinyMetroGameModeBase::SetGameSpeed(float TimeDilation) {
    UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TimeDilation);
    if (!IsValid(PlayerController)) PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!IsValid(PawnRef)) PawnRef = PlayerController->GetPawn();
    PlayerController->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
    PawnRef->CustomTimeDilation = 1 / UGameplayStatics::GetGlobalTimeDilation(GetWorld());
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

AInvestmentManager* ATinyMetroGameModeBase::GetInvestmentManager() const {
    return InvestmentManagerRef;
}

ASoundManager* ATinyMetroGameModeBase::GetSoundManager() const {
    return SoundManagerRef;
}

void ATinyMetroGameModeBase::TestFunction() {
    UInvestmentLuaState* InvestmentLuaState = UInvestmentLuaState::CreateInstance(GetWorld());
    UEventLuaState* EventLuaState = UEventLuaState::CreateInstance(GetWorld());

    FString ScriptDirectory = TEXT("Script");
    ScriptDirectory.Append(FGenericPlatformMisc::GetDefaultPathSeparator());

    FString ScriptFileName = TEXT("TestScript.lua");
    ScriptDirectory.Append(ScriptFileName);

    auto readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(GetWorld(), InvestmentLuaState->GetSelfLuaState(),
        ScriptDirectory, false);

    auto readLuaEvent = ULuaBlueprintFunctionLibrary::LuaRunFile(GetWorld(), EventLuaState->GetSelfLuaState(),
        ScriptDirectory, false);

    auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(GetWorld(), EventLuaState->GetClass(),
        TEXT("TestFunction"), TArray<FLuaValue>());
}
