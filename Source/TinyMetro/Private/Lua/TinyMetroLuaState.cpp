// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/TinyMetroLuaState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Timer/Timer.h"
#include "Statistics/StatisticsManager.h"
#include "Station/StationManager.h"
#include "Train/TrainManager.h"
#include "Train/Train.h"
#include "Policy/Policy.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UTinyMetroLuaState::UTinyMetroLuaState() {
	bRawLuaFunctionCall = true;

    Table.Add(TEXT("AddIncome"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UTinyMetroLuaState, AddIncome, FLuaValue)));
    Table.Add(TEXT("AddMoney"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UTinyMetroLuaState, AddMoney, FLuaValue)));
    Table.Add(TEXT("AddItem"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_TwoParams(UTinyMetroLuaState, AddItem, FLuaValue, FLuaValue)));

    Table.Add(TEXT("GetTimestamp"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetTimestamp)));
    Table.Add(TEXT("GetPolicyData"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetPolicyData)));
    Table.Add(TEXT("GetDefaultStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetDefaultStatistics)));
    Table.Add(TEXT("GetLaneStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetLaneStatistics)));
    Table.Add(TEXT("GetLaneDetailStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetLaneDetailStatistics)));
    Table.Add(TEXT("GetShopStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetShopStatistics)));
    Table.Add(TEXT("GetTrainStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetTrainStatistics)));
    Table.Add(TEXT("GetSubtrainStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetSubtrainStatistics)));
    Table.Add(TEXT("GetBridgeStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetBridgeStatistics)));
    Table.Add(TEXT("GetTunnelStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetTunnelStatistics)));
    Table.Add(TEXT("GetBankStatistics"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetBankStatistics)));
    Table.Add(TEXT("GetStationInfos"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetStationInfos)));
    Table.Add(TEXT("GetTrainInfos"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UTinyMetroLuaState, GetTrainInfos)));
}

UTinyMetroLuaState* UTinyMetroLuaState::CreateInstance(UWorld* WorldContextObject) {
    UTinyMetroLuaState* tmp = NewObject<UTinyMetroLuaState>();
    ULuaBlueprintFunctionLibrary::LuaCreateTable(WorldContextObject, tmp->GetSelfLuaState());

    return tmp;
}

void UTinyMetroLuaState::InitReferClasses() {
    if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
    if (!IsValid(PlayerStateRef)) PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
    if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();
    if (!IsValid(StationManagerRef)) StationManagerRef = GameModeRef->GetStationManager();
    if (!IsValid(TrainManagerRef)) TrainManagerRef = GameModeRef->GetTrainManager();
    if (!IsValid(PolicyRef)) PolicyRef = GameModeRef->GetPolicy();
}

void UTinyMetroLuaState::AddIncome(FLuaValue Money) {
    InitReferClasses();
    PlayerStateRef->AddIncome(Money.ToInteger());
}

void UTinyMetroLuaState::AddMoney(FLuaValue Money) {
    InitReferClasses();
    PlayerStateRef->AddMoney(Money.ToInteger());
}

void UTinyMetroLuaState::AddItem(FLuaValue Item, FLuaValue Amount) {
    InitReferClasses();
    FString itemName = Item.ToString().ToUpper();

    if (itemName == TEXT("TRAIN")) {
        PlayerStateRef->AddItem(ItemType::Train, Amount.ToInteger());
    } else if (itemName == TEXT("SUBTRAIN")) {
        PlayerStateRef->AddItem(ItemType::Subtrain, Amount.ToInteger());
    } else if (itemName == TEXT("LANE")) {
        PlayerStateRef->AddItem(ItemType::Lane, Amount.ToInteger());
    } else if (itemName == TEXT("BRIDGE")) {
        PlayerStateRef->AddItem(ItemType::Bridge, Amount.ToInteger());
    } else if (itemName == TEXT("TUNNEL")) {
        PlayerStateRef->AddItem(ItemType::Tunnel, Amount.ToInteger());
    }
}

FLuaValue UTinyMetroLuaState::GetTimestamp() {
    InitReferClasses();
    FLuaValue timeTable = CreateLuaTable();
    auto timestamp = TimerRef->GetTimestamp();
    
    timeTable.SetField(TEXT("Date"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(timestamp.Date));
    const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("Day"), true);
    FString dayName = enumPtr->GetNameStringByValue((uint8)timestamp.DayoftheWeek);
    timeTable.SetField(TEXT("DayoftheWeek"), ULuaBlueprintFunctionLibrary::LuaCreateString(dayName));
    timeTable.SetField(TEXT("Week"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(timestamp.Week));

    return timeTable;
}

FLuaValue UTinyMetroLuaState::GetPolicyData() {
    InitReferClasses();
    FLuaValue policyTable = CreateLuaTable();
    auto policyData = PolicyRef->GetPolicyData();

    policyTable.SetField(TEXT("ServiceLevel"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(policyData.ServiceCostLevel));
    policyTable.SetField(TEXT("IsPrioritySeat"), ULuaBlueprintFunctionLibrary::LuaCreateBool(policyData.PrioritySeat));
    policyTable.SetField(TEXT("IsCCTV"), ULuaBlueprintFunctionLibrary::LuaCreateBool(policyData.HasCCTV));
    policyTable.SetField(TEXT("IsElevator"), ULuaBlueprintFunctionLibrary::LuaCreateBool(policyData.HasElevator));
    policyTable.SetField(TEXT("IsBicycle"), ULuaBlueprintFunctionLibrary::LuaCreateBool(policyData.HasBicycle));
    policyTable.SetField(TEXT("IsTransfer"), ULuaBlueprintFunctionLibrary::LuaCreateBool(policyData.HasTransfer));

    return policyTable;
}

// Return StatisticsManager::DefaultStatistics
FLuaValue UTinyMetroLuaState::GetDefaultStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto defaultStatistics = StatisticsManagerRef->DefaultStatistics;

    statisticsTable.SetField(TEXT("TotalArrivePassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.TotalArrivePassenger));
    statisticsTable.SetField(TEXT("WeeklyArrivePassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.WeeklyArrivePassenger));
    statisticsTable.SetField(TEXT("TotalIncome"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.TotalIncome));
    statisticsTable.SetField(TEXT("WeeklyIncome"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.WeeklyIncome));
    statisticsTable.SetField(TEXT("TotalSpending"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.TotalSpending));
    statisticsTable.SetField(TEXT("WeeklySpending"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.WeeklySpending));
    statisticsTable.SetField(TEXT("AverageComplain"), ULuaBlueprintFunctionLibrary::LuaCreateNumber(defaultStatistics.AverageComplain));
    statisticsTable.SetField(TEXT("ServiceStationCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.ServiceStationCount));
    statisticsTable.SetField(TEXT("UpgradeStationCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.UpgradeStationCount));

    FLuaValue test;

    return statisticsTable;
}

// Return StatisticsManager::LaneStatistics
FLuaValue UTinyMetroLuaState::GetLaneStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto laneStatistics = StatisticsManagerRef->LaneStatistics;

    statisticsTable.SetField(TEXT("TotalLaneCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.TotalLaneCount));
    statisticsTable.SetField(TEXT("TotalModifyAndDeleteCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.TotalModifyAndDeleteCount));
    
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

    statisticsTable.SetField(TEXT("TotalUsingMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TotalUsingMoney));
    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TotalPurchaseCount));

    return statisticsTable;
}

FLuaValue UTinyMetroLuaState::GetTrainStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalPlacementCount));
    statisticsTable.SetField(TEXT("TotalShiftCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalShiftCount));
    statisticsTable.SetField(TEXT("TotalRetrievalCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalRetrievalCount));
    statisticsTable.SetField(TEXT("TotalUpgradeCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalUpgradeCount));

    return statisticsTable;
}

FLuaValue UTinyMetroLuaState::GetSubtrainStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalPlacementCount));
    statisticsTable.SetField(TEXT("TotalShiftCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalShiftCount));
    statisticsTable.SetField(TEXT("TotalRetrievalCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalRetrievalCount));
    statisticsTable.SetField(TEXT("TotalUpgradeCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalUpgradeCount));

    return FLuaValue();
}

FLuaValue UTinyMetroLuaState::GetBridgeStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.BridgeStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.BridgeStatistics.TotalPlacementCount));

    return FLuaValue();
}

FLuaValue UTinyMetroLuaState::GetTunnelStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = StatisticsManagerRef->ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TunnelStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TunnelStatistics.TotalPlacementCount));

    return FLuaValue();
}

// Return StatisticsManager::BankStatistics
FLuaValue UTinyMetroLuaState::GetBankStatistics() {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto bankStatistics = StatisticsManagerRef->BankStatistics;

    statisticsTable.SetField(TEXT("TotalUsingLoanCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalUsingLoanCount));
    statisticsTable.SetField(TEXT("TotalRepayMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalRepayMoney));
    statisticsTable.SetField(TEXT("TotalLoanMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalLoanMoney));
    statisticsTable.SetField(TEXT("TotalRepayInterestMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalRepayInterestMoney));

    return statisticsTable;
}

TArray<FLuaValue> UTinyMetroLuaState::GetStationInfos() {
    InitReferClasses();
    TArray<FLuaValue> infoTable;
    auto stationInfos = StationManagerRef->GetAllStationInfo();

    for (auto& i : stationInfos) {
        FLuaValue tmp = CreateLuaTable();
        tmp.SetField(TEXT("Id"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.Id));
        tmp.SetField(TEXT("ServiceLaneCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.ServiceLaneCount));
        tmp.SetField(TEXT("TotalUsingPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.TotalUsingPassenger));
        tmp.SetField(TEXT("WeeklyUsingPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.WeeklyUsingPassenger));
        tmp.SetField(TEXT("TotalProfit"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.TotalProfit));
        tmp.SetField(TEXT("WeeklyProfit"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.WeeklyProfit));
        tmp.SetField(TEXT("Complain"), ULuaBlueprintFunctionLibrary::LuaCreateNumber(i.Complain));
        tmp.SetField(TEXT("GetOnPassengerCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.GetOnPassengerCount));
        tmp.SetField(TEXT("TotalWaitTime"), ULuaBlueprintFunctionLibrary::LuaCreateNumber(i.TotalWaitTime));
        tmp.SetField(TEXT("AverageWaitTime"), ULuaBlueprintFunctionLibrary::LuaCreateNumber(i.AverageWaitTime));
        tmp.SetField(TEXT("TotalTransferPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.TotalTransferPassenger));
        tmp.SetField(TEXT("WeeklyTransferPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.WeeklyTransferPassenger));
        tmp.SetField(TEXT("IsActive"), ULuaBlueprintFunctionLibrary::LuaCreateBool(i.IsActive));
        tmp.SetField(TEXT("IsDestroyed"), ULuaBlueprintFunctionLibrary::LuaCreateBool(i.IsDestroyed));
    }

    return infoTable;
}

TArray<FLuaValue> UTinyMetroLuaState::GetTrainInfos() {
    InitReferClasses();
    TArray<FLuaValue> infoTable;

    for (auto& i : TrainManagerRef->GetAllTrain()) {
        auto trainInfo = i->GetTrainInfo();
        FLuaValue tmp = CreateLuaTable();
        tmp.SetField(TEXT("Id"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(trainInfo.Id));
        tmp.SetField(TEXT("ServiceLaneId"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(trainInfo.ServiceLaneId));
        if (trainInfo.Type == TrainType::Train) {
            tmp.SetField(TEXT("TrainType"), ULuaBlueprintFunctionLibrary::LuaCreateString(TEXT("Train")));
            tmp.SetField(TEXT("SubtrainCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(Cast<ATrain>(i)->GetSubtrainCount()));
        }else {
            tmp.SetField(TEXT("TrainType"), ULuaBlueprintFunctionLibrary::LuaCreateString(TEXT("Subtrain")));
            tmp.SetField(TEXT("SubtrainCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(-1));
        }
        tmp.SetField(TEXT("TotalPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(trainInfo.TotalBoardPassenger));
        tmp.SetField(TEXT("WeeklyPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(trainInfo.WeeklyBoardPassenger));
        tmp.SetField(TEXT("ShiftCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(trainInfo.ShiftCount));
        tmp.SetField(TEXT("IsUpgrade"), ULuaBlueprintFunctionLibrary::LuaCreateBool(trainInfo.IsUpgrade));
    }

    return infoTable;
}
