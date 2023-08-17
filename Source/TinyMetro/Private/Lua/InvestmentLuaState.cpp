// Fill out your copyright notice in the Description page of Project Settings.


#include "Lua/InvestmentLuaState.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

UInvestmentLuaState::UInvestmentLuaState() {
    Table.Add(TEXT("InvestmentLuaStateTest"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, InvestmentLuaStateTest, FLuaValue)));

    Table.Add(TEXT("AddIncome"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddIncome, FLuaValue)));
    Table.Add(TEXT("AddMoney"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_OneParam(UInvestmentLuaState, AddMoney, FLuaValue)));
    Table.Add(TEXT("AddItem"), FLuaValue::Function(GET_FUNCTION_NAME_CHECKED_TwoParams(UInvestmentLuaState, AddItem, FLuaValue, FLuaValue)));

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
