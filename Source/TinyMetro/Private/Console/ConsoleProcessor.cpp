// Fill out your copyright notice in the Description page of Project Settings.


#include "Console/ConsoleProcessor.h"
#include "Station/StationManager.h"
#include "GridGenerator/GridManager.h"
#include "Finance/Bank.h"
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
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	BankRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetBank();
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
	FString result = TEXT("Add Money : ");
	if (Cmd.IsValidIndex(1)) {
		if (Cmd[1].IsNumeric()) {
			PlayerStateRef->AddMoney(FCString::Atoi(*Cmd[1]));
			Success = true;
		} else {
			Success = false;
		}
	} else {
		Success = false;
	}
	result += Success ? TEXT("Success") : TEXT("Fail");
	return result;
}

FString AConsoleProcessor::CmdOccurEvent(TArray<FString> Cmd, bool& Success) {
	return FString();
}

// add_station : Spawn random station at random point
// add_station {station type} : Spawn {station type} station at random point
// add_station {x} {y} : Spawn random station at {x}, {y} point
// add_station {x} {y} {station type} : Spawn {station type} station at {x}, {y} point
FString AConsoleProcessor::CmdAddStation(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Add Station : ");
	StationType sType;
	switch (Cmd.Num()) {
	case 1: // add_station
		StationManagerRef->SpawnStation(
			GridManagerRef->GetGridCellDataRandom(),
			StationManagerRef->GetRandomStationType(),
			false
		);
		result += TEXT("Success");
		break;
	case 2: // add_station {station type}
		sType = StationManagerRef->StationTypeFromString(Cmd[1], Success);
		if (Success) {
			StationManagerRef->SpawnStation(
				GridManagerRef->GetGridCellDataRandom(),
				sType,
				false
			);
			result += TEXT("Success");
		} else {
			result += TEXT("Incorrect input");
		}
		break;
	case 3: // add_station {x} {y}
		if (Cmd[1].IsNumeric() && Cmd[2].IsNumeric()) {
			StationManagerRef->SpawnStation(
				GridManagerRef->GetGridCellDataByPoint(FCString::Atoi(*Cmd[1]), FCString::Atoi(*Cmd[2])),
				StationManagerRef->GetRandomStationType(),
				false
			);
			result += TEXT("Success");
		} else {
			Success = false;
			result += TEXT("Incorrect input");
		}
		break;
	case 4: // add_station {x} {y} {station type}
		sType = StationManagerRef->StationTypeFromString(Cmd[3], Success);
		if (Cmd[1].IsNumeric() && Cmd[2].IsNumeric() && Success) {
			StationManagerRef->SpawnStation(
				GridManagerRef->GetGridCellDataByPoint(FCString::Atoi(*Cmd[1]), FCString::Atoi(*Cmd[2])),
				sType,
				false
			);
			result += TEXT("Success");
		} else {
			Success = false;
			result += TEXT("Incorrect input");
		}
		break;
	default:
		Success = false;
		result += TEXT("Incorrect input");
		break;
	}
	return result;
}

FString AConsoleProcessor::CmdDeleteStation(TArray<FString> Cmd, bool& Success) {
	return FString();
}

FString AConsoleProcessor::CmdDeleteLane(TArray<FString> Cmd, bool& Success) {
	return FString();
}

// investment_clear : Success all investment
// investment_clear {n} : Success {n}th investment
FString AConsoleProcessor::CmdInvestmentSuccess(TArray<FString> Cmd, bool& Success) {
	// TODO : After finish investment
	//FString result = TEXT("Investment clear : ");
	//switch (Cmd.Num()) {
	//case 1: // investment_clear
	//	for (auto& i : BankRef->GetAvailableInvestment()) {

	//	}
	//	break;
	//case 2: // investment_clear {n}
	//	if (Cmd[1].IsNumeric()) {

	//	} else {
	//		Success = false;
	//		result += TEXT("Fail");
	//	}
	//	break;
	//default: // Fail
	//	Success = false;
	//	result += TEXT("Fail");
	//}
	return FString();
}

// investment_fail : Fail all investment
// investment_fail {n} : Fail {n}th investment
FString AConsoleProcessor::CmdInvestmentFail(TArray<FString> Cmd, bool& Success) {
	return FString();
}

// repay : Repay all loan
// repay {id} : Repay loan selected {id}
FString AConsoleProcessor::CmdRepay(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Repay : ");
	switch (Cmd.Num()) {
	case 1: // repay
		break;
	case 2: // repay {id}
		if (Cmd[1].IsNumeric()) {

		} else {
			Success = false;
			result += TEXT("Incorrect input");
		}
		break;
	default: // Incorrect input
		Success = false;
		result += TEXT("Incorrect input");
		break;
	}

	return result;
}

FString AConsoleProcessor::CmdComplainOn(TArray<FString> Cmd, bool& Success) {
	return FString();
}

// complain_off : Stop increase complain all stations
// compalin_off {id} Stop increase complain {id}'s station
FString AConsoleProcessor::CmdComplainOff(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Complain off : ");
	switch (Cmd.Num()) {
	case 1: // complain_off
		for (auto& i : StationManagerRef->GetAllStations()) {
			i->SetComplainIncreaseEnable(false);
		}
		break;
	case 2: // complain_off {id}
		if (Cmd[1].IsNumeric()) {
			auto stationRef = StationManagerRef->GetStationById(FCString::Atoi(*Cmd[1]));
			if (IsValid(stationRef)) {
				stationRef->SetComplainIncreaseEnable(false);
			} else {
				Success = false;
				result += TEXT("Invalid station");
			}
		} else {
			Success = false;
			result += TEXT("Incorrect input");
		}
		break;
	default: // Incorrect input
		Success = false;
		result += TEXT("Incorrect input");
		break;
	}
	return result;
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
	Success = true; // Initialize
	for (auto& i : splitStr) {
		i = i.ToLower();
	}
	if (arrSize == 0) {
		Success = false;
		return TEXT("Null command");
	} else {
		if (splitStr[0] == TEXT("passenger_info")) {
			Result = CmdPassengerInfo(splitStr, Success);
		} else if (splitStr[0] == TEXT("money")) {
			Result = CmdMoney(splitStr, Success);
		} else if (splitStr[0] == TEXT("event")) {
			// TODO : event command
		} else if (splitStr[0] == TEXT("add_station")) {
			Result = CmdAddStation(splitStr, Success);
		} else if (splitStr[0] == TEXT("del_station")) {
			// TODO : del_station command
		} else if (splitStr[0] == TEXT("del_lane")) {
			// TODO : del_lane command
		} else if (splitStr[0] == TEXT("investment_clear")) {
			// TODO : investment_clear command
		} else if (splitStr[0] == TEXT("investment_fail")) {
			// TODO : investment_fail command
		} else if (splitStr[0] == TEXT("repay")) {
			Result = CmdRepay(splitStr, Success);
		} else if (splitStr[0] == TEXT("complain_off")) {
			Result = CmdComplainOff(splitStr, Success);
		}
	}

	Success = true;
	return Result;
}
