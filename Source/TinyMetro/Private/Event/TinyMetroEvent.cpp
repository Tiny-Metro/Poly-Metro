// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/TinyMetroEvent.h"
#include "Lua/EventLuaState.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"

UTinyMetroEvent::UTinyMetroEvent() {
	ScriptDirectory = TEXT("Script");
	ScriptDirectory.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Event"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Saved"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
}

UTinyMetroEvent* UTinyMetroEvent::CreateEvent(FString ScriptFileName, UEventLuaState* LuaState, UWorld* WorldContextObject) {
	UTinyMetroEvent* Obj = NewObject<UTinyMetroEvent>();
	Obj->ScriptFileName = ScriptFileName;
	Obj->LuaState = LuaState;
	Obj->WorldRef = WorldContextObject;
	Obj->InitLuaState();
	
	return Obj;
}

void UTinyMetroEvent::InitLuaState() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(WorldRef, LuaState->GetSelfLuaState(),
		ScriptDirectory + ScriptFileName, false);
}

int32 UTinyMetroEvent::EventStart() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Start"), TArray<FLuaValue>());
	InitEvent();

	return GetEventPeriod();
}

void UTinyMetroEvent::InitEvent() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("EventData"), TArray<FLuaValue>());
	TArray<FString> parseArr;
	ScriptFileName.ParseIntoArray(parseArr, TEXT("."));

	EventData.Id = FCString::Atoi(*parseArr[1]);
	EventData.Title = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("title")));
	EventData.FlavorText = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("flavor_text")));
	EventData.Message = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(readLua.GetField(TEXT("message")));
	EventData.Period = readLua.GetField(TEXT("period")).ToInteger();
}

int32 UTinyMetroEvent::GetEventPeriod() {
	return EventData.Period;
}

int32 UTinyMetroEvent::GetEventWeight() {
	auto readLua = ULuaBlueprintFunctionLibrary::LuaGlobalCall(WorldRef, LuaState->GetClass(),
		TEXT("Weight"), TArray<FLuaValue>());

	return readLua.ToInteger();
}

FTinyMetroEventData UTinyMetroEvent::GetEventData() const {
	return EventData;
}
