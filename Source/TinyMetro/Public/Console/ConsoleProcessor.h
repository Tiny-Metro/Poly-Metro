// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConsoleProcessor.generated.h"

UCLASS()
class TINYMETRO_API AConsoleProcessor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConsoleProcessor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void TextTest(FText Txt);
	void StringTest(FString Str);

	FString CmdPassengerInfo(TArray<FString> Cmd, bool& Success);
	FString CmdMoney(TArray<FString> Cmd, bool& Success);
	FString CmdOccurEvent(TArray<FString> Cmd, bool& Success);
	FString CmdAddStation(TArray<FString> Cmd, bool& Success);
	FString CmdDeleteStation(TArray<FString> Cmd, bool& Success);
	FString CmdDeleteLane(TArray<FString> Cmd, bool& Success);
	FString CmdInvestmentSuccess(TArray<FString> Cmd, bool& Success);
	FString CmdInvestmentFail(TArray<FString> Cmd, bool& Success);
	FString CmdRepay(TArray<FString> Cmd, bool& Success);
	FString CmdComplainOn(TArray<FString> Cmd, bool& Success);
	FString CmdComplainOff(TArray<FString> Cmd, bool& Success);
	FString CmdComplainAdd(TArray<FString> Cmd, bool& Success);
	FString CmdSkipTime(TArray<FString> Cmd, bool& Success);
	FString CmdAddPassenger(TArray<FString> Cmd, bool& Success);
	FString CmdDeletePassenger(TArray<FString> Cmd, bool& Success);
	FString CmdTogglePassengerSpawn(TArray<FString> Cmd, bool& Success);

	UFUNCTION(BlueprintCallable)
	FString Command(FString Cmd, bool& Success);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	class AStationManager* StationManagerRef;
};
