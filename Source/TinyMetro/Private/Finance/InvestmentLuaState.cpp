// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/InvestmentLuaState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Policy/Policy.h"
#include "Station/StationManager.h"
#include "Lane/LaneManager.h"
#include "Train/TrainManager.h"
#include "Finance/InvestmentState.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UInvestmentLuaState::UInvestmentLuaState() {
    //Table.Add(TEXT("get_number"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UMyLuaState, GetNumber)));
    //Table.Add(TEXT("SomeValue"), FLuaValue(777));
    bRawLuaFunctionCall = true;
    
    // API : Player
    Table.Add(TEXT("AddMoney"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddMoney, FLuaValue)));

    // API : Policy
    Table.Add(TEXT("GetServiceLevel"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetServiceLevel)));
    Table.Add(TEXT("SetServiceLevel"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, SetServiceLevel, FLuaValue)));
    Table.Add(TEXT("GetPrioritySeat"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetPrioritySeat)));
    Table.Add(TEXT("SetPrioritySeat"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, SetPrioritySeat, FLuaValue)));
    Table.Add(TEXT("GetCCTV"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetCCTV)));
    Table.Add(TEXT("SetCCTV"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, SetCCTV, FLuaValue)));
    Table.Add(TEXT("GetElevator"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetElevator)));
    Table.Add(TEXT("SetElevator"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, SetElevator, FLuaValue)));
    Table.Add(TEXT("GetBicycle"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetBicycle)));
    Table.Add(TEXT("SetBicycle"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, SetBicycle, FLuaValue)));
    Table.Add(TEXT("GetTransfer"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetTransfer)));
    Table.Add(TEXT("SetTransfer"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, SetTransfer, FLuaValue)));

    // API : Station
    Table.Add(TEXT("GetStationCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetStationCount)));
    Table.Add(TEXT("GetActiveStationCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetActiveStationCount)));
    Table.Add(TEXT("GetInactiveStationCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetInactiveStationCount)));
    Table.Add(TEXT("GetDestroyedStationCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetDestroyedStationCount)));
    Table.Add(TEXT("GetTypeStationCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTypeStationCount, FLuaValue)));
    Table.Add(TEXT("GetRandomStationId"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetRandomStationId)));
    Table.Add(TEXT("GetRandomActiveStationId"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetRandomActiveStationId)));
    Table.Add(TEXT("GetRandomInactiveStationId"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetRandomInactiveStationId)));
    Table.Add(TEXT("GetRandomDestroyedStationId"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetRandomDestroyedStationId)));
    Table.Add(TEXT("GetRandomTypeStationId"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetRandomTypeStationId, FLuaValue)));
    Table.Add(TEXT("AddComplainFixedValue"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_TwoParams(UInvestmentLuaState, AddComplainFixedValue, FLuaValue, FLuaValue)));
    Table.Add(TEXT("AddComplainByRate"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_TwoParams(UInvestmentLuaState, AddComplainByRate, FLuaValue, FLuaValue)));

    // API : Passenger
    Table.Add(TEXT("GetTotalPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTotalPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetFreePassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetFreePassengerCount, FLuaValue)));
    Table.Add(TEXT("GetPaidPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetPaidPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetArriveTotalPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetArriveTotalPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetArriveFreePassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetArriveFreePassengerCount, FLuaValue)));
    Table.Add(TEXT("GetArrivePaidPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetArrivePaidPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetWaitTotalPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetWaitTotalPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetWaitFreePassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetWaitFreePassengerCount, FLuaValue)));
    Table.Add(TEXT("GetWaitPaidPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetWaitPaidPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetDestroyedTotalPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetDestroyedTotalPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetDestroyedFreePassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetDestroyedFreePassengerCount, FLuaValue)));
    Table.Add(TEXT("GetDestroyedPaidPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetDestroyedPaidPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetRideTotalPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetRideTotalPassengerCount, FLuaValue)));
    Table.Add(TEXT("GetRideFreePassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetRideFreePassengerCount, FLuaValue)));
    Table.Add(TEXT("GetRidePaidPassengerCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetRidePaidPassengerCount, FLuaValue)));
    Table.Add(TEXT("AddPassengerSpawn"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddPassengerSpawn, FLuaValue)));

    // API : Lane
    Table.Add(TEXT("GetLaneCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetLaneCount)));
    Table.Add(TEXT("GetRandomLaneId"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetRandomLaneId)));

    // API : Train
    Table.Add(TEXT("GetTrainCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetTrainCount)));
    Table.Add(TEXT("GetUpgradeTrainCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetUpgradeTrainCount)));
    Table.Add(TEXT("GetMainTrainCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetMainTrainCount)));
    Table.Add(TEXT("GetUpgradeMainTrainCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetUpgradeMainTrainCount)));
    Table.Add(TEXT("GetSubTrainCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetSubTrainCount)));
    Table.Add(TEXT("GetUpgradeSubTrainCount"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UInvestmentLuaState, GetUpgradeSubTrainCount)));
    Table.Add(TEXT("GetTrainCountByLane"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetTrainCountByLane, FLuaValue)));
    Table.Add(TEXT("GetUpgradeTrainCountByLane"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetUpgradeTrainCountByLane, FLuaValue)));
    Table.Add(TEXT("GetMainTrainCountByLane"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetMainTrainCountByLane, FLuaValue)));
    Table.Add(TEXT("GetUpgradeMainTrainCountByLane"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetUpgradeMainTrainCountByLane, FLuaValue)));
    Table.Add(TEXT("GetSubTrainCountByLane"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetSubTrainCountByLane, FLuaValue)));
    Table.Add(TEXT("GetUpgradeSubTrainCountByLane"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, GetUpgradeSubTrainCountByLane, FLuaValue)));

    // InvestmentState
    Table.Add(TEXT("Ready"), FLuaValue(0));
    Table.Add(TEXT("Process"), FLuaValue(1));
    Table.Add(TEXT("Success"), FLuaValue(2));
    Table.Add(TEXT("Fail"), FLuaValue(3));

    // Test Table
    Table.Add(TEXT(""), FLuaValue(0));
}

UInvestmentLuaState* UInvestmentLuaState::CreateInstance(UWorld* WorldContextObject) {
    UInvestmentLuaState* tmp = NewObject<UInvestmentLuaState>();
    ULuaBlueprintFunctionLibrary::LuaCreateTable(WorldContextObject, tmp->GetSelfLuaState());
    
    return tmp;
}

void UInvestmentLuaState::AddMoney(FLuaValue Money) {
    if (!IsValid(PlayerStateRef)) {
        PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
    }
    PlayerStateRef->AddMoney(Money.ToInteger());
}

FLuaValue UInvestmentLuaState::GetServiceLevel() const {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    return FLuaValue(PolicyRef->GetServiceCostLevel());
}

FLuaValue UInvestmentLuaState::GetPrioritySeat() const {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    return FLuaValue(PolicyRef->GetPrioritySeat());
}

FLuaValue UInvestmentLuaState::GetCCTV() const {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    return FLuaValue(PolicyRef->GetCCTV());
}

FLuaValue UInvestmentLuaState::GetElevator() const {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    return FLuaValue(PolicyRef->GetElevator());
}

FLuaValue UInvestmentLuaState::GetBicycle() const {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    return FLuaValue(PolicyRef->GetBicycle());
}

FLuaValue UInvestmentLuaState::GetTransfer() const {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    return FLuaValue(PolicyRef->GetTransfer());
}

void UInvestmentLuaState::SetServiceLevel(FLuaValue Level) {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    PolicyRef->SetServiceCostLevel(Level.ToInteger());
}

void UInvestmentLuaState::SetPrioritySeat(FLuaValue Flag) {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    PolicyRef->SetPrioritySeat(Flag.ToBool());
}

void UInvestmentLuaState::SetCCTV(FLuaValue Flag) {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    PolicyRef->SetCCTV(Flag.ToBool());
}

void UInvestmentLuaState::SetElevator(FLuaValue Flag) {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    PolicyRef->SetElevator(Flag.ToBool());
}

void UInvestmentLuaState::SetBicycle(FLuaValue Flag) {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    PolicyRef->SetBicycle(Flag.ToBool());
}

void UInvestmentLuaState::SetTransfer(FLuaValue Flag) {
    if (!IsValid(PolicyRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        PolicyRef = GameModeRef->GetPolicy();
    }
    PolicyRef->SetTransfer(Flag.ToBool());
}

FLuaValue UInvestmentLuaState::GetStationCount() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetStationCount());
}

FLuaValue UInvestmentLuaState::GetActiveStationCount() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetActiveStationCount());
}

FLuaValue UInvestmentLuaState::GetInactiveStationCount() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetInactiveStationCount());
}

FLuaValue UInvestmentLuaState::GetDestroyedStationCount() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetDestroyedStationCount());
}

FLuaValue UInvestmentLuaState::GetTypeStationCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetTypeStationCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetRandomStationType() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(AStationManager::StationTypeToString(StationManagerRef->GetRandomStationType(), success));
}

FLuaValue UInvestmentLuaState::GetRandomStationId() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetRandomStationInfo().Id);
}

FLuaValue UInvestmentLuaState::GetRandomActiveStationId() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetRandomActiveStationInfo().Id);
}

FLuaValue UInvestmentLuaState::GetRandomInactiveStationId() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetRandomInactiveStationInfo().Id);
}

FLuaValue UInvestmentLuaState::GetRandomDestroyedStationId() const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    return FLuaValue(StationManagerRef->GetRandomDestroyedStationInfo().Id);
}

// TODO : pull by develop
FLuaValue UInvestmentLuaState::GetRandomTypeStationId(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetRandomTypeStationInfo(AStationManager::StationTypeFromString(Type.ToString(), success)).Id);
}

void UInvestmentLuaState::AddComplainFixedValue(FLuaValue StationId, FLuaValue Complain) {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    StationManagerRef->GetStationById(StationId.ToInteger())->AddComplain(Complain.ToInteger());
}

void UInvestmentLuaState::AddComplainByRate(FLuaValue StationId, FLuaValue Rate) {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    StationManagerRef->GetStationById(StationId.ToInteger())->AddComplain(Rate.ToFloat());
}

FLuaValue UInvestmentLuaState::GetTotalPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetTotalPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetFreePassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetFreePassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetPaidPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetPaidPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetArriveTotalPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetArriveTotalPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetArriveFreePassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetArriveFreePassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetArrivePaidPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetArrivePaidPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetWaitTotalPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetWaitTotalPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetWaitFreePassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetWaitFreePassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetWaitPaidPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetWaitPaidPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetDestroyedTotalPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetDestroyedTotalPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetDestroyedFreePassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetDestroyedFreePassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetDestroyedPaidPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetDestroyedPaidPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetRideTotalPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetRideTotalPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetRideFreePassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetRideFreePassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

FLuaValue UInvestmentLuaState::GetRidePaidPassengerCount(FLuaValue Type) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    bool success;
    return FLuaValue(StationManagerRef->GetRidePaidPassengerCount(AStationManager::StationTypeFromString(Type.ToString(), success)));
}

void UInvestmentLuaState::AddPassengerSpawn(FLuaValue Rate) const {
    if (!IsValid(StationManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        StationManagerRef = GameModeRef->GetStationManager();
    }
    // TODO : Imple in StationManager
    //StationManagerRef->AddPassengerSpawnSpeed(Rate.ToFloat());
}

FLuaValue UInvestmentLuaState::GetLaneCount() const {
    if (!IsValid(LaneManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        LaneManagerRef = GameModeRef->GetLaneManager();
    }
    return FLuaValue(LaneManagerRef->GetLaneCount());
}

FLuaValue UInvestmentLuaState::GetRandomLaneId() const {
    if (!IsValid(LaneManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        LaneManagerRef = GameModeRef->GetLaneManager();
    }
    return FLuaValue(LaneManagerRef->GetRandomLaneId());
}

FLuaValue UInvestmentLuaState::GetTrainCount() const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetTrainCount());
}

FLuaValue UInvestmentLuaState::GetUpgradeTrainCount() const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetTrainCountFilterByUpgrade(true));
}

FLuaValue UInvestmentLuaState::GetMainTrainCount() const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetMainTrainCount());
}

FLuaValue UInvestmentLuaState::GetUpgradeMainTrainCount() const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetMainTrainCountFilterByUpgrade(true));
}

FLuaValue UInvestmentLuaState::GetSubTrainCount() const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetSubTrainCount());
}

FLuaValue UInvestmentLuaState::GetUpgradeSubTrainCount() const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetSubTrainCountFilterByUpgrade(true));
}

FLuaValue UInvestmentLuaState::GetTrainCountByLane(FLuaValue Id) const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetTrainCount(Id.ToInteger()));
}

FLuaValue UInvestmentLuaState::GetUpgradeTrainCountByLane(FLuaValue Id) const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetTrainCountFilterByUpgrade(true, Id.ToInteger()));
}

FLuaValue UInvestmentLuaState::GetMainTrainCountByLane(FLuaValue Id) const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetMainTrainCount(Id.ToInteger()));
}

FLuaValue UInvestmentLuaState::GetUpgradeMainTrainCountByLane(FLuaValue Id) const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetMainTrainCountFilterByUpgrade(true, Id.ToInteger()));
}

FLuaValue UInvestmentLuaState::GetSubTrainCountByLane(FLuaValue Id) const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetSubTrainCount(Id.ToInteger()));
}

FLuaValue UInvestmentLuaState::GetUpgradeSubTrainCountByLane(FLuaValue Id) const {
    if (!IsValid(TrainManagerRef)) {
        if (!IsValid(GameModeRef)) {
            GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
        }
        TrainManagerRef = GameModeRef->GetTrainManager();
    }
    return FLuaValue(TrainManagerRef->GetSubTrainCountFilterByUpgrade(true, Id.ToInteger()));
}
