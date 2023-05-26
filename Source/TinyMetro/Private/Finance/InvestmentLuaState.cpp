// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/InvestmentLuaState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Policy/Policy.h"
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
