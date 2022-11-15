// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GameModeBaseSeoul.h"

FString AGameModeBaseSeoul::GetFileName() const {
    return TEXT("Seoul");
}

TMap<FIntPoint, StationType> AGameModeBaseSeoul::GetInitData() const {
    TMap<FIntPoint, StationType> Tmp;
    Tmp.Add(FIntPoint(27, 16), StationType::Triangle);
    Tmp.Add(FIntPoint(32, 21), StationType::Square);
    Tmp.Add(FIntPoint(39, 12), StationType::Circle);
    return Tmp;
}

