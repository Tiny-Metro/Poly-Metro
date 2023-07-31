// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/TinyMetroLuaState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Statistics/StatisticsManager.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UTinyMetroLuaState::UTinyMetroLuaState() {
	bRawLuaFunctionCall = true;

    Table.Add(TEXT("GetDefaultStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetDefaultStatistics)));
    Table.Add(TEXT("GetLaneStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetLaneStatistics)));
    Table.Add(TEXT("GetLaneDetailStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetLaneDetailStatistics)));
    Table.Add(TEXT("GetShopStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetShopStatistics)));
    Table.Add(TEXT("GetTrainStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetTrainStatistics)));
    Table.Add(TEXT("GetSubtrainStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetSubtrainStatistics)));
    Table.Add(TEXT("GetBridgeStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetBridgeStatistics)));
    Table.Add(TEXT("GetTunnelStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetTunnelStatistics)));
    Table.Add(TEXT("GetBankStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetBankStatistics)));
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

    FLuaValue test;

    return statisticsTable;
}

// Return StatisticsManager::LaneStatistics
FLuaValue UTinyMetroLuaState::GetLaneStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto laneStatistics = StatisticsManagerRef->LaneStatistics;

    statisticsTable.SetField(TEXT("TotalLaneCount"), FLuaValue(laneStatistics.TotalLaneCount));
    statisticsTable.SetField(TEXT("TotalModifyAndDeleteCount"), FLuaValue(laneStatistics.TotalModifyAndDeleteCount));
    
    return FLuaValue();
}

TArray<FLuaValue> UTinyMetroLuaState::GetLaneDetailStatistics() {
    InitReferClasses();
    TArray<FLuaValue> Result;
    Result.Add(FLuaValue());
    auto laneStatistics = StatisticsManagerRef->LaneStatistics;

    // Add lane 1 ~ 8
    for (int i = 1; i < 9; i++) {
        FLuaValue tmp = CreateLuaTable();
        tmp.SetField(TEXT("TotalArrivePassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].TotalArrivePassenger));
        tmp.SetField(TEXT("WeeklyArrivePassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].WeeklyArrivePassenger));
        tmp.SetField(TEXT("TotalProfit"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].TotalProfit));
        tmp.SetField(TEXT("WeeklyProfit"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].WeeklyProfit));
        tmp.SetField(TEXT("UsingBridgeCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].UsingBridgeCount));
        tmp.SetField(TEXT("UsingTunnelCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].UsingTunnelCount));
        tmp.SetField(TEXT("TransferStationCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].TransferStationCount));
        tmp.SetField(TEXT("ServiceStationCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].ServiceStationCount));
        tmp.SetField(TEXT("TotalModifyAndReduceCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].TotalModifyAndReduceCount));
        tmp.SetField(TEXT("AverageComplain"), ULuaBlueprintFunctionLibrary::LuaCreateNumber(laneStatistics.Lanes[i].AverageComplain));
        tmp.SetField(TEXT("ServiceTrainAndSubtrainCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.Lanes[i].ServiceTrainAndSubtrainCount));
        tmp.SetField(TEXT("IsCircularLane"), ULuaBlueprintFunctionLibrary::LuaCreateBool(laneStatistics.Lanes[i].IsCircularLane));

        Result.Add(tmp);
    }

    return Result;
}

// Return StatisticsManager::ShopStatistics
FLuaValue UTinyMetroLuaState::GetShopStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalUsingMoney"), FLuaValue(shopStatistics.TotalUsingMoney));
    statisticsTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.TotalPurchaseCount));

    return statisticsTable;
}

FLuaValue UTinyMetroLuaState::GetTrainStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.TrainStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.TrainStatistics.TotalPlacementCount));
    statisticsTable.SetField(TEXT("TotalShiftCount"), FLuaValue(shopStatistics.TrainStatistics.TotalShiftCount));
    statisticsTable.SetField(TEXT("TotalRetrievalCount"), FLuaValue(shopStatistics.TrainStatistics.TotalRetrievalCount));
    statisticsTable.SetField(TEXT("TotalUpgradeCount"), FLuaValue(shopStatistics.TrainStatistics.TotalUpgradeCount));

    return statisticsTable;
}

FLuaValue UTinyMetroLuaState::GetSubtrainStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalPlacementCount));
    statisticsTable.SetField(TEXT("TotalShiftCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalShiftCount));
    statisticsTable.SetField(TEXT("TotalRetrievalCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalRetrievalCount));
    statisticsTable.SetField(TEXT("TotalUpgradeCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalUpgradeCount));

    return FLuaValue();
}

FLuaValue UTinyMetroLuaState::GetBridgeStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.BridgeStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.BridgeStatistics.TotalPlacementCount));

    return FLuaValue();
}

FLuaValue UTinyMetroLuaState::GetTunnelStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.TunnelStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.TunnelStatistics.TotalPlacementCount));

    return FLuaValue();
}

// Return StatisticsManager::BankStatistics
FLuaValue UTinyMetroLuaState::GetBankStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto bankStatistics = StatisticsManagerRef->BankStatistics;

    statisticsTable.SetField(TEXT("TotalUsingLoanCount"), FLuaValue(bankStatistics.TotalUsingLoanCount));
    statisticsTable.SetField(TEXT("TotalRepayMoney"), FLuaValue(bankStatistics.TotalRepayMoney));
    statisticsTable.SetField(TEXT("TotalLoanMoney"), FLuaValue(bankStatistics.TotalLoanMoney));
    statisticsTable.SetField(TEXT("TotalRepayInterestMoney"), FLuaValue(bankStatistics.TotalRepayInterestMoney));

    return statisticsTable;
}
