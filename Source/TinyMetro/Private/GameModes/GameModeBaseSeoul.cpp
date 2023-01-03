// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GameModeBaseSeoul.h"
#include "PlayerState/PlayerStateSeoul.h"
#include "Kismet/GameplayStatics.h"

AGameModeBaseSeoul::AGameModeBaseSeoul() {
    PlayerStateClass = APlayerStateSeoul::StaticClass();
}

void AGameModeBaseSeoul::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;
    FRotator rotator = FRotator::ZeroRotator;
    FVector spawnLocation = FVector::ZeroVector;

    Timer = GetWorld()->SpawnActor<ATimer>();
    TMSaveManager = GetWorld()->SpawnActor<ATMSaveManager>();
}

FString AGameModeBaseSeoul::GetFileName() const {
    return TEXT("Seoul");
}

TMap<FIntPoint, StationType> AGameModeBaseSeoul::GetInitData() const {
    TMap<FIntPoint, StationType> Tmp;
    if (!UGameplayStatics::DoesSaveGameExist("StationSave", 0))
    {
        Tmp.Add(FIntPoint(27, 16), StationType::Triangle);
        Tmp.Add(FIntPoint(32, 21), StationType::Square);
        Tmp.Add(FIntPoint(39, 12), StationType::Circle);
    }

    return Tmp;
}

int32 AGameModeBaseSeoul::GetDaytime() const {
    return 12;
}


float AGameModeBaseSeoul::GetTime() {

    return Timer->ElapseTime;
}

void AGameModeBaseSeoul::SetTime(float elapseTime) {

    if (Timer == nullptr || Timer == NULL) {
        UE_LOG(LogTemp, Error, TEXT("Timer is nullptr!"));
        return;
    }
    else {
        Timer->ElapseTime = elapseTime;
    }

    
}