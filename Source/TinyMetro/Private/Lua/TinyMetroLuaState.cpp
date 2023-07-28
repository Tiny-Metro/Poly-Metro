// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/TinyMetroLuaState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Statistics/StatisticsManager.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UTinyMetroLuaState::UTinyMetroLuaState() {
	bRawLuaFunctionCall = true;
}

UTinyMetroLuaState* UTinyMetroLuaState::CreateInstance(UWorld* WorldContextObject) {
    UTinyMetroLuaState* tmp = NewObject<UTinyMetroLuaState>();
    ULuaBlueprintFunctionLibrary::LuaCreateTable(WorldContextObject, tmp->GetSelfLuaState());

    return tmp;
}

void UTinyMetroLuaState::InitReferClasses() {
    if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();
}

// Return StatisticsManager::DefaultStatistics
FLuaValue UTinyMetroLuaState::GetDefaultStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto defaultStatistics = StatisticsManagerRef->DefaultStatistics;

    statisticsTable.SetField(TEXT("TotalArrivePassenger"), FLuaValue(defaultStatistics.TotalArrivePassenger));
    statisticsTable.SetField(TEXT("WeeklyArrivePassenger"), FLuaValue(defaultStatistics.WeeklyArrivePassenger));
    statisticsTable.SetField(TEXT("TotalIncome"), FLuaValue(defaultStatistics.TotalIncome));
    statisticsTable.SetField(TEXT("WeeklyIncome"), FLuaValue(defaultStatistics.WeeklyIncome));
    statisticsTable.SetField(TEXT("TotalSpending"), FLuaValue(defaultStatistics.TotalSpending));
    statisticsTable.SetField(TEXT("WeeklySpending"), FLuaValue(defaultStatistics.WeeklySpending));
    statisticsTable.SetField(TEXT("AverageComplain"), FLuaValue(defaultStatistics.AverageComplain));
    statisticsTable.SetField(TEXT("ServiceStationCount"), FLuaValue(defaultStatistics.ServiceStationCount));
    statisticsTable.SetField(TEXT("UpgradeStationCount"), FLuaValue(defaultStatistics.UpgradeStationCount));

    return statisticsTable;
}
