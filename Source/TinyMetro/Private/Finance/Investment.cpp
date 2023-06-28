// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Investment.h"
#include "Finance/InvestmentLuaState.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"

UInvestment::UInvestment() {
	ScriptDirectory = TEXT("Script");
	ScriptDirectory.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
	ScriptDirectory.Append(TEXT("Investment"));
	ScriptDirectory.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
}

UInvestment* UInvestment::CreateInvestment(FString ScriptFileName, UInvestmentLuaState* LuaState, UWorld* WorldContextObject){
	UInvestment* Obj = NewObject<UInvestment>();
	Obj->ScriptFileName = ScriptFileName;
	Obj->LuaState = LuaState;
	Obj->WorldRef = WorldContextObject;
	Obj->InitInvestment();

	return Obj;
}

void UInvestment::InvestmentStart() {
	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Start"), TArray<FLuaValue>());
	State = InvestmentState::Process;
}

InvestmentState UInvestment::InvestmentProcess() {
	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Process"), TArray<FLuaValue>());
	
	// LuaValue to int
	luaFunction.ToInteger();

	return InvestmentState::Process;
}

InvestmentState UInvestment::InvestmentFinish() {
	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Finish"), TArray<FLuaValue>());
	return InvestmentState::Success;
}

void UInvestment::InvestmentSuccess() {
	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("OnSuccess"), TArray<FLuaValue>());
}

bool UInvestment::GetAppearable() const {
	return true;
}

void UInvestment::InitInvestment() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(WorldRef, LuaState->GetSelfLuaState(),
		ScriptDirectory.Append(ScriptFileName), false);

	InvestmentData = FInvestmentData(
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("title"))),
		readLua.GetField(TEXT("time_limit")).ToInteger(),
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("award_message")))
	);

	ResetInvestment();
}

void UInvestment::ResetInvestment() {
	State = InvestmentState::Ready;
	RemainTime = InvestmentData.TimeLimit;
}


void UInvestment::NotifyDailyTask() {
	if (State == InvestmentState::Process) {
		RemainTime--;
	}
}

FInvestmentData UInvestment::GetInvestmentData() const {
	return InvestmentData;
}

InvestmentState UInvestment::GetState() const {
	return State;
}
