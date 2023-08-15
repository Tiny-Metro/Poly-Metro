// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/EventLuaState.h"
#include "GridGenerator/GridManager.h"
#include "Station/StationManager.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UEventLuaState::UEventLuaState() {
    Table.Add(TEXT("AddStation"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UEventLuaState, AddStation, FLuaValue)));
    Table.Add(TEXT("DestroyStation"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UEventLuaState, DestroyStation, FLuaValue)));
    Table.Add(TEXT("AddPassengerSpawnProbability"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UEventLuaState, AddPassengerSpawnProbability, FLuaValue)));
    Table.Add(TEXT("AddPassengerSpawnProbabilityByType"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_TwoParams(UEventLuaState, AddPassengerSpawnProbabilityByType, FLuaValue, FLuaValue)));
    Table.Add(TEXT("ScaleComplain"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UEventLuaState, ScaleComplain, FLuaValue)));
    Table.Add(TEXT("AddComplainIncreaseRate"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UEventLuaState, AddComplainIncreaseRate, FLuaValue)));

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
    InitReferClasses();
    bool tmp;

    StationManagerRef->AddPassengerDestinationTypeWeight(
        AStationManager::StationTypeFromString(Type.ToString(), tmp),
        Amount.ToFloat());
}

void UEventLuaState::ScaleComplain(FLuaValue ScaleFactor) {
    InitReferClasses();

    for (auto& i : StationManagerRef->GetAllStations()) {
        i->ScaleComplain(ScaleFactor.ToFloat());
    }
}

void UEventLuaState::AddComplainIncreaseRate(FLuaValue Rate) {
    InitReferClasses();

    for (auto& i : StationManagerRef->GetAllStations()) {
        i->AddComplainIncreaseRate(Rate.ToFloat());
    }
}
