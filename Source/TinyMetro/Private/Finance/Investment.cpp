// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Investment.h"
#include "Finance/InvestmentManager.h"
#include "Finance/InvestmentSaveGame.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "SaveSystem/TMSaveManager.h"
#include "Timer/Timer.h"
#include "Lua/InvestmentLuaState.h"
#include <Kismet/GameplayStatics.h>
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"

void UInvestment::BeginPlay() {
	Super::BeginPlay();

	// Init class references
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(InvestmentManagerRef)) InvestmentManagerRef = GameModeRef->GetInvestmentManager();
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();

	// Init data
	auto investmentData = LuaCallFunction(TEXT("InvestmentData"), TArray<FLuaValue>(), false);
	InvestmentId = LuaGetField(TEXT("Id")).ToInteger();
	Message = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(investmentData.GetField(TEXT("message")));
	TimeRequire = investmentData.GetField(TEXT("time_require")).ToInteger();
	AwardMessage = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(investmentData.GetField(TEXT("award")));
	// Log : Investment data
	UE_LOG(LogTemp, Log, TEXT("Investment::BeginPlay, %d : %s"), InvestmentId, *Message);

	InitInvestment();

	Load();

	TimerRef->DailyTask.AddDynamic(this, &UInvestment::DailyTask);
	SaveManagerRef->SaveTask.AddDynamic(this, &UInvestment::Save);
}

UInvestment::UInvestment() : Super() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UInvestment::InitInvestment() {
	// Reset investment state
	IsStart = false;
	State = InvestmentState::Ready;
	RemainTime = 0;
}

void UInvestment::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsStart) {
		auto luaProcess = LuaCallFunction(TEXT("Process"), TArray<FLuaValue>(), false);
		FString processResult = luaProcess.ToString();
		{
			FScopeLock Lock(MutexKey.Get());

			if (processResult == TEXT("continue")) {
				return;
			} else if (processResult == TEXT("success")) {
				Success();
			} else if (processResult == TEXT("fail")) {
				Fail();
			}
		}
	}
}

void UInvestment::Start() {
	InvestmentManagerRef->NotifyInvestmentStart(InvestmentId);
	LuaCallFunction(TEXT("Start"), TArray<FLuaValue>(), false);
	IsStart = true;
	State = InvestmentState::Processing;
}

void UInvestment::Success() {
	InvestmentManagerRef->NotifyInvestmentFinish(InvestmentId);
	LuaCallFunction(TEXT("Award"), TArray<FLuaValue>(), false);
	IsStart = false;
	State = InvestmentState::Ready;
}

void UInvestment::Fail() {
	InvestmentManagerRef->NotifyInvestmentFinish(InvestmentId);
	IsStart = false;
	State = InvestmentState::Ready;
}

void UInvestment::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UInvestmentSaveGame* tmp = Cast<UInvestmentSaveGame>(UGameplayStatics::CreateSaveGameObject(UInvestmentSaveGame::StaticClass()));

	tmp->IsStart = IsStart;
	tmp->RemainTime = RemainTime;
	tmp->State = State;

	SaveManagerRef->Save(tmp, SaveActorType::Investment, InvestmentId);
}

void UInvestment::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UInvestmentSaveGame* tmp = Cast<UInvestmentSaveGame>(SaveManagerRef->Load(SaveActorType::Investment, InvestmentId));

	if (!IsValid(tmp)) {
		return;
	}

	IsStart = tmp->IsStart;
	RemainTime = tmp->RemainTime;
	State = tmp->State;
}

void UInvestment::DailyTask() {
	// Update remain time
	if (IsStart) {
		RemainTime--;
		// Fail when time out
		if (RemainTime < 0) Fail();
	}
}

bool UInvestment::GetAppearance() {
	return LuaCallFunction(TEXT("Appearance"), TArray<FLuaValue>(), false).ToBool();
}

InvestmentState UInvestment::GetState() {
	return State;
}

//UInvestment* UInvestment::CreateInvestment(FString ScriptFileName, UInvestmentLuaState* LuaState, UWorld* WorldContextObject){
//	UInvestment* Obj = NewObject<UInvestment>();
//	Obj->ScriptFileName = ScriptFileName;
//	Obj->LuaStateRef = LuaState;
//	Obj->WorldRef = WorldContextObject;
//	Obj->InitLuaState();
//	Obj->InitInvestment();
//
//	return Obj;
//}
//
//void UInvestment::InitLuaState() {
//	auto readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(WorldRef, LuaStateRef->GetSelfLuaState(),
//		ScriptDirectory.Append(ScriptFileName), false);
//}
//
//void UInvestment::InitInvestment() {
//	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaStateRef->GetClass(),
//		TEXT("InvestmentData"), TArray<FLuaValue>());
//	TArray<FString> parseArr;
//	ScriptFileName.ParseIntoArray(parseArr, TEXT("."));
//
//	InvestmentData.Id = FCString::Atoi(*parseArr[1]);
//	InvestmentData.Message = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("message")));
//	InvestmentData.TimeRequire = readLua.GetField(TEXT("time_require")).ToInteger();
//	InvestmentData.Award = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("award")));
//
//	ResetInvestment();
//}
//
//void UInvestment::ResetInvestment() {
//	InvestmentData.State = InvestmentState::Ready;
//	RemainTime = InvestmentData.TimeRequire;
//}
//
//void UInvestment::InvestmentStart() {
//	auto luaFunction = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaStateRef->GetClass(),
//		TEXT("Start"), TArray<FLuaValue>());
//	InvestmentData.State = InvestmentState::Processing;
//}
//
//void UInvestment::InvestmentProcess() {
//	if (InvestmentData.State == InvestmentState::Processing) {
//		UE_LOG(LogTemp,Log,TEXT("Processing..."))
//		auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaStateRef->GetClass(),
//			TEXT("Process"), TArray<FLuaValue>());
//
//		// Result is { success, fail, continue }
//		FString processResult = readLua.ToString();
//
//		{
//			FScopeLock Lock(MutexKey.Get());
//
//			if (processResult == TEXT("continue")) {
//				return;
//			} else if (processResult == TEXT("success")) {
//				InvestmentSuccess();
//			} else if (processResult == TEXT("fail")) {
//				InvestmentFail();
//			}
//		}
//	}
//}
//
//void UInvestment::InvestmentSuccess() {
//	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaStateRef->GetClass(),
//		TEXT("Award"), TArray<FLuaValue>());
//	InvestmentData.State = InvestmentState::Success;
//}
//
//void UInvestment::InvestmentFail() {
//	InvestmentData.State = InvestmentState::Fail;
//}
//
//bool UInvestment::GetAppearance() {
//	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaStateRef->GetClass(),
//		TEXT("Appearance"), TArray<FLuaValue>());
//	return readLua.ToBool();
//}
//
//void UInvestment::NotifyDailyTask() {
//	if (InvestmentData.State == InvestmentState::Processing) {
//		ElapseTime += Daytime;
//	}
//}
//
//FInvestmentData UInvestment::GetInvestmentData() const {
//	return InvestmentData;
//}
//
//InvestmentState UInvestment::GetInvestmentState() const {
//	return InvestmentData.State;
//}
