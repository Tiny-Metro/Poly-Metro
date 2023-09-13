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

FString AGameModeBaseSeoul::GetMapName() const {
    // TODO : Get game language setting, sepearte return value
    return TEXT("서울");
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

TMap<int32, FColor> AGameModeBaseSeoul::GetLaneColor() const {
    TMap<int32, FColor> Colors;

    Colors.Add(0, FColor::FromHex(TEXT("8A8A8AFF")));
    Colors.Add(1, FColor::FromHex(TEXT("0052A4FF")));
    Colors.Add(2, FColor::FromHex(TEXT("00A84DFF")));
    Colors.Add(3, FColor::FromHex(TEXT("EF7C1CFF")));
    Colors.Add(4, FColor::FromHex(TEXT("00A5DEFF")));
    Colors.Add(5, FColor::FromHex(TEXT("996CACFF")));
    Colors.Add(6, FColor::FromHex(TEXT("F5A200FF")));
    Colors.Add(7, FColor::FromHex(TEXT("77C4A3FF")));
    Colors.Add(8, FColor::FromHex(TEXT("E6186CFF")));
    return Colors;
}

void AGameModeBaseSeoul::BeginPlay() {
    Super::BeginPlay();
}
