// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/EventLuaState.h"
#include "GridGenerator/GridManager.h"
#include "Station/StationManager.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UEventLuaState::UEventLuaState() {
}

UEventLuaState* UEventLuaState::CreateInstance(UWorld* WorldContextObject) {
	UEventLuaState* tmp = NewObject<UEventLuaState>();
	ULuaBlueprintFunctionLibrary::LuaCreateTable(WorldContextObject, tmp->GetSelfLuaState());

	return tmp;
}

void UEventLuaState::AddStation(FLuaValue Amount) {
    InitReferClasses();

    for (int i = 0; i < Amount.ToInteger(); i++) {
        StationManagerRef->SpawnStation(
            GridManagerRef->GetGridCellDataRandom(),
            StationManagerRef->GetRandomStationType());
    }
}

void UEventLuaState::DestroyStation(FLuaValue Amount) {
    InitReferClasses();

    for (int i = 0; i < Amount.ToInteger(); i++) {
        StationManagerRef->DestroyRandomStation();
    }
}

void UEventLuaState::AddPassengerSpawnProbability(FLuaValue Amount) {
    InitReferClasses();

    StationManagerRef->AddPassegnerSpawnProbabilityByEvent(Amount.ToFloat());
}

void UEventLuaState::AddPassengerSpawnProbabilityByType(FLuaValue Type, FLuaValue Amount) {
}

void UEventLuaState::ScaleComplain(FLuaValue ScaleFactor) {
}

void UEventLuaState::AddComplainIncreaseRate(FLuaValue Rate) {
}
