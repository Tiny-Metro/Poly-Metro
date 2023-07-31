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
    Table.Add(TEXT("GetShopStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetShopStatistics)));
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
    for (int i = 1; i < 9; i++) {
        FLuaValue tempTable = CreateLuaTable();
        tempTable.SetField(TEXT("TotalArrivePassenger"), FLuaValue(laneStatistics.Lanes[i].TotalArrivePassenger));
        tempTable.SetField(TEXT("WeeklyArrivePassenger"), FLuaValue(laneStatistics.Lanes[i].WeeklyArrivePassenger));
        tempTable.SetField(TEXT("TotalProfit"), FLuaValue(laneStatistics.Lanes[i].TotalProfit));
        tempTable.SetField(TEXT("WeeklyProfit"), FLuaValue(laneStatistics.Lanes[i].WeeklyProfit));
        tempTable.SetField(TEXT("UsingBridgeCount"), FLuaValue(laneStatistics.Lanes[i].UsingBridgeCount));
        tempTable.SetField(TEXT("UsingTunnelCount"), FLuaValue(laneStatistics.Lanes[i].UsingTunnelCount));
        tempTable.SetField(TEXT("TransferStationCount"), FLuaValue(laneStatistics.Lanes[i].TransferStationCount));
        tempTable.SetField(TEXT("ServiceStationCount"), FLuaValue(laneStatistics.Lanes[i].ServiceStationCount));
        tempTable.SetField(TEXT("TotalModifyAndReduceCount"), FLuaValue(laneStatistics.Lanes[i].TotalModifyAndReduceCount));
        tempTable.SetField(TEXT("AverageComplain"), FLuaValue(laneStatistics.Lanes[i].AverageComplain));
        tempTable.SetField(TEXT("ServiceTrainAndSubtrainCount"), FLuaValue(laneStatistics.Lanes[i].ServiceTrainAndSubtrainCount));
        tempTable.SetField(TEXT("IsCircularLane"), FLuaValue(laneStatistics.Lanes[i].IsCircularLane));

        statisticsTable.SetField(FString::Printf(TEXT("Lane%d"), i), tempTable);
    }
    
    return FLuaValue();
}

// Return StatisticsManager::ShopStatistics
FLuaValue UTinyMetroLuaState::GetShopStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    FLuaValue trainTable = CreateLuaTable();
    FLuaValue subtrainTable = CreateLuaTable();
    FLuaValue bridgeTable = CreateLuaTable();
    FLuaValue tunnelTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    trainTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.TrainStatistics.TotalPurchaseCount));
    trainTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.TrainStatistics.TotalPlacementCount));
    trainTable.SetField(TEXT("TotalShiftCount"), FLuaValue(shopStatistics.TrainStatistics.TotalShiftCount));
    trainTable.SetField(TEXT("TotalRetrievalCount"), FLuaValue(shopStatistics.TrainStatistics.TotalRetrievalCount));
    trainTable.SetField(TEXT("TotalUpgradeCount"), FLuaValue(shopStatistics.TrainStatistics.TotalUpgradeCount));

    subtrainTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalPurchaseCount));
    subtrainTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalPlacementCount));
    subtrainTable.SetField(TEXT("TotalShiftCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalShiftCount));
    subtrainTable.SetField(TEXT("TotalRetrievalCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalRetrievalCount));
    subtrainTable.SetField(TEXT("TotalUpgradeCount"), FLuaValue(shopStatistics.SubtrainStatistics.TotalUpgradeCount));

    bridgeTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.BridgeStatistics.TotalPurchaseCount));
    bridgeTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.BridgeStatistics.TotalPlacementCount));

    tunnelTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.TunnelStatistics.TotalPurchaseCount));
    tunnelTable.SetField(TEXT("TotalPlacementCount"), FLuaValue(shopStatistics.TunnelStatistics.TotalPlacementCount));

    statisticsTable.SetField(TEXT("TotalUsingMoney"), FLuaValue(shopStatistics.TotalUsingMoney));
    statisticsTable.SetField(TEXT("TotalPurchaseCount"), FLuaValue(shopStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TrainStatistics"), trainTable);
    statisticsTable.SetField(TEXT("SubtrainStatistics"), subtrainTable);
    statisticsTable.SetField(TEXT("BridgeStatistics"), bridgeTable);
    statisticsTable.SetField(TEXT("TunnelStatistics"), tunnelTable);

    return statisticsTable;
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
