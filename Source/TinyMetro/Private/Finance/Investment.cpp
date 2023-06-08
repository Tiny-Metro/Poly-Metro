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

void UInvestment::SetDaytime(int32 T) {
	Daytime = T;
}

void UInvestment::SetInvestmentData(FInvestmentData Data) {
	InvestmentData = Data;
}

void UInvestment::SetAcceptAction(TFunction<void(void)> Action) {
	StartAction = Action;
}

void UInvestment::SetSuccessAction(TFunction<void(void)> Action) {
	SuccessAction = Action;
}

void UInvestment::SetFailAction(TFunction<void(void)> Action) {
	FailAction = Action;
}

void UInvestment::SetStateCheckFunction(TFunction<InvestmentState(void)> Check) {
	ConditionCheckFunction = Check;
}

void UInvestment::InvestmentStart() {
	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("InvestmentStart"), TArray<FLuaValue>());
	State = InvestmentState::Processing;
}

void UInvestment::InitInvestment() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(WorldRef, LuaState->GetSelfLuaState(),
		ScriptDirectory.Append(ScriptFileName), false);

	InvestmentData = FInvestmentData(
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("title"))),
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("content"))),
		readLua.GetField(TEXT("time_require")).ToInteger(),
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("require_message"))),
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("accept_message"))),
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("award_message"))),
		ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("reward_message")))
	);

	ResetInvestment();

	/*auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetSelfLuaState(),
		TEXT("Test"), TArray<FLuaValue>());*/
	
	/*if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
			FString::Printf(TEXT("%s : %s, Title : %s"), *ScriptFileName, *luaFunction.ToString(), *ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("title")))));
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("title"))));
	}*/
}

void UInvestment::ResetInvestment() {
	State = InvestmentState::Ready;
	RemainTime = InvestmentData.TimeRequire;
}

InvestmentState UInvestment::CheckInvestmentProcess(float DeltaTime) {
	if (State == InvestmentState::Processing) {
		switch (ConditionCheckFunction()) {
		case InvestmentState::Fail:
			FailAction();
			return InvestmentState::Fail;
		case InvestmentState::Success:
			if (ElapseTime >= Daytime * InvestmentData.TimeRequire) { // Time over : Success
				SuccessAction();
				return InvestmentState::Success;
			} else { // Process investment
				return InvestmentState::Processing;
			}
		default:
			return InvestmentState::Processing;
		}
	}

	return InvestmentState::Ready;
}

void UInvestment::NotifyDailyTask() {
	if (State == InvestmentState::Processing) {
		ElapseTime += Daytime;
	}
}

FInvestmentData UInvestment::GetInvestmentData() const {
	return InvestmentData;
}

InvestmentState UInvestment::GetState() const {
	return State;
}
