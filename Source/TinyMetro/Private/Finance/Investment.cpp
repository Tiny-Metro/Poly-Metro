// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Investment.h"
#include "Lua/InvestmentLuaState.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"

UInvestment::UInvestment() {
	ScriptDirectory = TEXT("Script");
	ScriptDirectory.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Investment"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Saved"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
}

UInvestment* UInvestment::CreateInvestment(FString ScriptFileName, UInvestmentLuaState* LuaState, UWorld* WorldContextObject){
	UInvestment* Obj = NewObject<UInvestment>();
	Obj->ScriptFileName = ScriptFileName;
	Obj->LuaState = LuaState;
	Obj->WorldRef = WorldContextObject;
	Obj->InitLuaState();
	Obj->InitInvestment();

	return Obj;
}

void UInvestment::InitLuaState() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(WorldRef, LuaState->GetSelfLuaState(),
		ScriptDirectory.Append(ScriptFileName), false);
}

void UInvestment::InitInvestment() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("InvestmentData"), TArray<FLuaValue>());
	TArray<FString> parseArr;
	ScriptFileName.ParseIntoArray(parseArr, TEXT("."));

	InvestmentData.Id = FCString::Atoi(*parseArr[1]);
	InvestmentData.Message = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("message")));
	InvestmentData.TimeRequire = readLua.GetField(TEXT("time_require")).ToInteger();
	InvestmentData.Award = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("award")));

	ResetInvestment();
}

void UInvestment::ResetInvestment() {
	InvestmentData.State = InvestmentState::Ready;
	RemainTime = InvestmentData.TimeRequire;
}

void UInvestment::InvestmentStart() {
	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Start"), TArray<FLuaValue>());
	InvestmentData.State = InvestmentState::Processing;
}

void UInvestment::InvestmentProcess() {
	if (InvestmentData.State == InvestmentState::Processing) {
		UE_LOG(LogTemp,Log,TEXT("Processing..."))
		auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
			TEXT("Process"), TArray<FLuaValue>());

		// Result is { success, fail, continue }
		FString processResult = readLua.ToString();

		{
			FScopeLock Lock(MutexKey.Get());

			if (processResult == TEXT("continue")) {
				return;
			} else if (processResult == TEXT("success")) {
				InvestmentSuccess();
			} else if (processResult == TEXT("fail")) {
				InvestmentFail();
			}
		}
	}
}

void UInvestment::InvestmentSuccess() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Award"), TArray<FLuaValue>());
	InvestmentData.State = InvestmentState::Success;
}

void UInvestment::InvestmentFail() {
	InvestmentData.State = InvestmentState::Fail;
}

bool UInvestment::GetAppearance() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Appearance"), TArray<FLuaValue>());
	return readLua.ToBool();
}

void UInvestment::NotifyDailyTask() {
	if (InvestmentData.State == InvestmentState::Processing) {
		ElapseTime += Daytime;
	}
}

FInvestmentData UInvestment::GetInvestmentData() const {
	return InvestmentData;
}

InvestmentState UInvestment::GetInvestmentState() const {
	return InvestmentData.State;
}
