// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/InvestmentLuaState.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Finance/InvestmentManager.h"
#include "Station/StationManager.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UInvestmentLuaState::UInvestmentLuaState() {
    Table.Add(TEXT("InvestmentLuaStateTest"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, InvestmentLuaStateTest, FLuaValue)));

    Table.Add(TEXT("AddIncome"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddIncome, FLuaValue)));
    Table.Add(TEXT("AddMoney"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddMoney, FLuaValue)));
    Table.Add(TEXT("AddItem"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_TwoParams(UInvestmentLuaState, AddItem, FLuaValue, FLuaValue)));

    Table.Add(TEXT("GetTimestampAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTimestampAtStart, FLuaValue)));
    Table.Add(TEXT("GetDefaultStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetDefaultStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetLaneStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetLaneStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetLaneDetailStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetLaneDetailStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetShopStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetShopStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetTrainStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTrainStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetSubtrainStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetSubtrainStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetBridgeStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetBridgeStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetTunnelStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTunnelStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetBankStatisticsAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetBankStatisticsAtStart, FLuaValue)));
    Table.Add(TEXT("GetStationInfosAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetStationInfosAtStart, FLuaValue)));
    Table.Add(TEXT("GetTrainInfosAtStart"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTrainInfosAtStart, FLuaValue)));

    Table.Add(TEXT("Train"), FLuaValue(TEXT("Train")));

    Table.Add(TEXT("success"), FLuaValue(TEXT("success")));
    Table.Add(TEXT("continue"), FLuaValue(TEXT("continue")));
    Table.Add(TEXT("fail"), FLuaValue(TEXT("fail")));
}

UInvestmentLuaState* UInvestmentLuaState::CreateInstance(UWorld* WorldContextObject) {
	UInvestmentLuaState* tmp = NewObject<UInvestmentLuaState>();
	ULuaBlueprintFunctionLibrary::LuaCreateTable(WorldContextObject, tmp->GetSelfLuaState());

	return tmp;
}

void UInvestmentLuaState::InvestmentLuaStateTest(FLuaValue Val) {
    InitReferClasses();
    UE_LOG(LogTemp, Log, TEXT("InvestmentLuaState::Test::%d"), Val.ToInteger());
}

void UInvestmentLuaState::AddIncome(FLuaValue Money) {
    InitReferClasses();
    PlayerStateRef->AddIncome(Money.ToInteger());
}

void UInvestmentLuaState::AddMoney(FLuaValue Money) {
    InitReferClasses();
    PlayerStateRef->AddMoney(Money.ToInteger());
}

void UInvestmentLuaState::AddItem(FLuaValue Item, FLuaValue Amount) {
    InitReferClasses();
    FString itemName = Item.ToString().ToUpper();
    UE_LOG(LogTemp, Log, TEXT("%s"), *itemName);

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

FLuaValue UInvestmentLuaState::GetTimestampAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue timeTable = CreateLuaTable();
    auto timestamp = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).Timestamp;

    timeTable.SetField(TEXT("Date"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(timestamp.Date));
    const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("Day"), true);
    FString dayName = enumPtr->GetNameStringByValue((uint8)timestamp.DayoftheWeek);
    timeTable.SetField(TEXT("DayoftheWeek"), ULuaBlueprintFunctionLibrary::LuaCreateString(dayName));
    timeTable.SetField(TEXT("Week"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(timestamp.Week));

    return timeTable;
}

FLuaValue UInvestmentLuaState::GetDefaultStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto defaultStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).DefaultStatistics;

    statisticsTable.SetField(TEXT("TotalArrivePassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.TotalArrivePassenger));
    statisticsTable.SetField(TEXT("WeeklyArrivePassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.WeeklyArrivePassenger));
    statisticsTable.SetField(TEXT("TotalIncome"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.TotalIncome));
    statisticsTable.SetField(TEXT("WeeklyIncome"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.WeeklyIncome));
    statisticsTable.SetField(TEXT("TotalSpending"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.TotalSpending));
    statisticsTable.SetField(TEXT("WeeklySpending"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.WeeklySpending));
    statisticsTable.SetField(TEXT("AverageComplain"), ULuaBlueprintFunctionLibrary::LuaCreateNumber(defaultStatistics.AverageComplain));
    statisticsTable.SetField(TEXT("ServiceStationCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.ServiceStationCount));
    statisticsTable.SetField(TEXT("UpgradeStationCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(defaultStatistics.UpgradeStationCount));

    return statisticsTable;
}

FLuaValue UInvestmentLuaState::GetLaneStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto laneStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).LaneStatistics;

    statisticsTable.SetField(TEXT("TotalLaneCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.TotalLaneCount));
    statisticsTable.SetField(TEXT("TotalModifyAndDeleteCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(laneStatistics.TotalModifyAndDeleteCount));

    return statisticsTable;
}

TArray<FLuaValue> UInvestmentLuaState::GetLaneDetailStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    TArray<FLuaValue> Result;
    Result.Add(FLuaValue());
    auto laneStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).LaneStatistics;

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

FLuaValue UInvestmentLuaState::GetShopStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).ShopStatistics;

    statisticsTable.SetField(TEXT("TotalUsingMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TotalUsingMoney));
    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TotalPurchaseCount));

    return statisticsTable;
}

FLuaValue UInvestmentLuaState::GetTrainStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalPlacementCount));
    statisticsTable.SetField(TEXT("TotalShiftCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalShiftCount));
    statisticsTable.SetField(TEXT("TotalRetrievalCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalRetrievalCount));
    statisticsTable.SetField(TEXT("TotalUpgradeCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TrainStatistics.TotalUpgradeCount));

    return statisticsTable;
}

FLuaValue UInvestmentLuaState::GetSubtrainStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalPlacementCount));
    statisticsTable.SetField(TEXT("TotalShiftCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalShiftCount));
    statisticsTable.SetField(TEXT("TotalRetrievalCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalRetrievalCount));
    statisticsTable.SetField(TEXT("TotalUpgradeCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.SubtrainStatistics.TotalUpgradeCount));

    return statisticsTable;
}

FLuaValue UInvestmentLuaState::GetBridgeStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.BridgeStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.BridgeStatistics.TotalPlacementCount));

    return statisticsTable;
}

FLuaValue UInvestmentLuaState::GetTunnelStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto shopStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).ShopStatistics;

    statisticsTable.SetField(TEXT("TotalPurchaseCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TunnelStatistics.TotalPurchaseCount));
    statisticsTable.SetField(TEXT("TotalPlacementCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(shopStatistics.TunnelStatistics.TotalPlacementCount));

    return statisticsTable;
}

FLuaValue UInvestmentLuaState::GetBankStatisticsAtStart(FLuaValue Id) {
    InitReferClasses();
    FLuaValue statisticsTable = CreateLuaTable();
    auto bankStatistics = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).BankStatistics;

    statisticsTable.SetField(TEXT("TotalUsingLoanCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalUsingLoanCount));
    statisticsTable.SetField(TEXT("TotalRepayMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalRepayMoney));
    statisticsTable.SetField(TEXT("TotalLoanMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalLoanMoney));
    statisticsTable.SetField(TEXT("TotalRepayInterestMoney"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(bankStatistics.TotalRepayInterestMoney));

    return statisticsTable;
}

TArray<FLuaValue> UInvestmentLuaState::GetStationInfosAtStart(FLuaValue Id) {
    InitReferClasses();
    TArray<FLuaValue> infoTable;
    auto stationInfos = InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).StationInfoArr;
    bool tmpBool;

    for (auto& i : stationInfos) {
        FLuaValue tmp = CreateLuaTable();
        tmp.SetField(TEXT("Id"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.Id));
        tmp.SetField(TEXT("StationType"), ULuaBlueprintFunctionLibrary::LuaCreateString(AStationManager::StationTypeToString(i.Type, tmpBool)));
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
        tmp.SetField(TEXT("IsUpgrade"), ULuaBlueprintFunctionLibrary::LuaCreateBool(i.IsUpgrade));
        infoTable.Add(tmp);
    }

    return infoTable;
}

TArray<FLuaValue> UInvestmentLuaState::GetTrainInfosAtStart(FLuaValue Id) {
    InitReferClasses();
    TArray<FLuaValue> infoTable;

    for (auto& i : InvestmentManagerRef->GetInvestmentStartData(Id.ToInteger()).TrainInfoArr) {
        FLuaValue tmp = CreateLuaTable();
        tmp.SetField(TEXT("Id"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.Id));
        tmp.SetField(TEXT("ServiceLaneId"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.ServiceLaneId));
        if (i.Type == TrainType::Train) {
            tmp.SetField(TEXT("TrainType"), ULuaBlueprintFunctionLibrary::LuaCreateString(TEXT("Train")));
            tmp.SetField(TEXT("SubtrainCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.SubtrainCount));
        } else {
            tmp.SetField(TEXT("TrainType"), ULuaBlueprintFunctionLibrary::LuaCreateString(TEXT("Subtrain")));
            tmp.SetField(TEXT("SubtrainCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(-1));
        }
        tmp.SetField(TEXT("TotalPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.TotalBoardPassenger));
        tmp.SetField(TEXT("WeeklyPassenger"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.WeeklyBoardPassenger));
        tmp.SetField(TEXT("ShiftCount"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(i.ShiftCount));
        tmp.SetField(TEXT("IsUpgrade"), ULuaBlueprintFunctionLibrary::LuaCreateBool(i.IsUpgrade));
        infoTable.Add(tmp);
    }

    return infoTable;
}
