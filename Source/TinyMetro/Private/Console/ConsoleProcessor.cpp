// Fill out your copyright notice in the Description page of Project Settings.


#include "Console/ConsoleProcessor.h"
#include "Station/StationManager.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AConsoleProcessor::AConsoleProcessor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AConsoleProcessor::BeginPlay()
{
	Super::BeginPlay();
	StationManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();

}

void AConsoleProcessor::TextTest(FText Txt) {
	
}

void AConsoleProcessor::StringTest(FString Str) {
	TArray<FString> outString;
	Str.ParseIntoArray(outString, TEXT(" "), true);
}

// Command : passenger_info {stationId}
// Return : Total Passenger : [], Wait Passenger : [], {Type Passengers} 
FString AConsoleProcessor::CmdPassengerInfo(TArray<FString> Cmd, bool& Success) {
	int32 TotalPassenger, WaitPassenger;
	TMap<StationType, int32> passengerStatistics;
	FString result = TEXT("");
	if (Cmd.Num() == 1) {
		Success = true;
		passengerStatistics = StationManagerRef->GetSpawnPassengerStatistics(TotalPassenger, WaitPassenger);
	} else {
		if (Cmd[1].IsNumeric()) {
			int32 stationId = FCString::Atoi(*Cmd[1]);
			passengerStatistics = StationManagerRef->GetSpawnPassengerStatistics(TotalPassenger, WaitPassenger, stationId);
			if (WaitPassenger == -1) {
				Success = false;
				result += TEXT("Invalid station ID");
			}
		} else {
			Success = false;
			result += TEXT("Invalid station ID");
		}
	}

	if (Success) {
		result += FString::Printf(TEXT("Total Passenger : %d\n"), TotalPassenger);
		result += FString::Printf(TEXT("Wait Passenger : %d\n"), WaitPassenger);
		const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("StationType"), true);
		for (auto& i : passengerStatistics) {
			result += enumPtr->GetNameStringByIndex((int32)i.Key);
			result += " : " + FString::FromInt(i.Value) + "\n";
		}
	}

	return result;
}

FString AConsoleProcessor::CmdMoney(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdOccurEvent(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdAddStation(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdDeleteStation(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdDeleteLane(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdInvestmentSuccess(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdInvestmentFail(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdRepay(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdComplainOn(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdComplainOff(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdComplainAdd(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdSkipTime(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdAddPassenger(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdDeletePassenger(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdTogglePassengerSpawn(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::Command(FString Cmd, bool& Success) {
	TArray<FString> splitStr;
	FString Result;
	int arrSize = Cmd.ParseIntoArray(splitStr, TEXT(" "));
	for (auto& i : splitStr) {
		i = i.ToLower();
	}
	if (arrSize == 0) {
		Success = false;
		return TEXT("Null command");
	} else {
		if (splitStr[0] == TEXT("passenger_info")) {
			Result = CmdPassengerInfo(splitStr, Success);
		} else if (splitStr[0] == TEXT("")) {

		}
	}

	Success = true;
	return Result;
}
