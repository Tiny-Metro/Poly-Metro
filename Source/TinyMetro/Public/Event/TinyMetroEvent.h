// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TinyMetroEventData.h"
#include "TinyMetroEvent.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UTinyMetroEvent : public UObject
{
	GENERATED_BODY()

public:
	UTinyMetroEvent();
	static UTinyMetroEvent* CreateEvent(FString ScriptFileName, class UEventLuaState* LuaState, UWorld* WorldContextObject);

	UFUNCTION()
	void InitLuaState();

	// Start event
	// Return event period
	UFUNCTION()
	int32 EventStart();

	UFUNCTION()
	void InitEvent();

	UFUNCTION()
	int32 GetEventPeriod();
	UFUNCTION()
	int32 GetEventWeight();
	UFUNCTION()
	FTinyMetroEventData GetEventData() const;

protected:
	UPROPERTY()
	UWorld* WorldRef;
	UPROPERTY()
	FString ScriptFileName;
	UPROPERTY()
	FString ScriptDirectory;
	UPROPERTY()
	class UEventLuaState* LuaState;
	UPROPERTY(BlueprintReadOnly)
	FTinyMetroEventData EventData;
	
};
