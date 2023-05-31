// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/InvestmentLuaState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Policy/Policy.h"
#include "Station/StationManager.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UInvestmentLuaState::UInvestmentLuaState() {
    //Table.Add(TEXT("get_number"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED(UMyLuaState, GetNumber)));
    //Table.Add(TEXT("SomeValue"), FLuaValue(777));
    bRawLuaFunctionCall = true;
    Table.Add(TEXT("AddMoney"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddMoney, FLuaValue)));
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
