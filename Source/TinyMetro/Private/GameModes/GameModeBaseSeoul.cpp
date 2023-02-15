// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GameModeBaseSeoul.h"
#include "PlayerState/PlayerStateSeoul.h"
#include "Kismet/GameplayStatics.h"

AGameModeBaseSeoul::AGameModeBaseSeoul() {
    PlayerStateClass = APlayerStateSeoul::StaticClass();
    
}


FString AGameModeBaseSeoul::GetFileName() const {
    return TEXT("Seoul");
}

TMap<FIntPoint, StationType> AGameModeBaseSeoul::GetInitData() const {
    TMap<FIntPoint, StationType> Tmp;
    if (!UGameplayStatics::DoesSaveGameExist("StationSave", 0))
    {
        Tmp.Add(FIntPoint(27, 22), StationType::Triangle);
        Tmp.Add(FIntPoint(32, 17), StationType::Rectangle);
        Tmp.Add(FIntPoint(39, 26), StationType::Circle);
    }

    return Tmp;
}

int32 AGameModeBaseSeoul::GetDaytime() const {
    return Daytime;
}

void AGameModeBaseSeoul::BeginPlay() {
    Super::BeginPlay();
}
