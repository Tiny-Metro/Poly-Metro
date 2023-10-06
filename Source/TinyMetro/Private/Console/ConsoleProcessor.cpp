// Fill out your copyright notice in the Description page of Project Settings.


#include "Console/ConsoleProcessor.h"
#include "Station/StationManager.h"
#include "GridGenerator/GridManager.h"
#include "Finance/Bank.h"
#include "Finance/InvestmentManager.h"
#include "Finance/Investment.h"
#include "Timer/Timer.h"
#include "Event/TinyMetroEventManager.h"
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
	auto GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	StationManagerRef = GameModeRef->GetStationManager();
	BankRef = GameModeRef->GetBank();
	TimerRef = GameModeRef->GetTimer();
	EventManagerRef = GameModeRef->GetEventManager();
	InvestmentManagerRef = GameModeRef->GetInvestmentManager();
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
	//int32 TotalPassenger, WaitPassenger;
	//TMap<StationType, int32> passengerStatistics;
	FString result = TEXT("Not implemented command");
	/*if (Cmd.Num() == 1) {
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
	}*/

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

// event : Occur random event
// evnet {event_id} : Occur {event_id} event
FString AConsoleProcessor::CmdOccurEvent(TArray<FString> Cmd, bool& Success) {
	if (Cmd.IsValidIndex(1)) {
		// event {event_id}
		int32 eventId = FCString::Atoi((*Cmd[1]));
		if (EventManagerRef->OccurSpecificEvent(eventId)) {
			Success = true;
			return FString::Printf(TEXT("Event %d is occured"), eventId);
		} else {
			Success = false;
			return TEXT("Invalid ID");
		}
	} else {
		// event
		EventManagerRef->UpdateEventWeight();
		EventManagerRef->OccurEvent();

		Success = true;
		return TEXT("Random event occured");
	}
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
	FString result = TEXT("Investment clear : ");
	switch (Cmd.Num()) {
	case 1: // investment_clear
		for (auto& i : InvestmentManagerRef->GetAllInvestment()) {
			if (i.Value->GetState() == InvestmentState::Processing) {
				i.Value->Success();
			}
		}
		result += TEXT("Command success : Clear all investment");
		break;
	case 2: // investment_clear {n}
		if (Cmd[1].IsNumeric()) {
			int32 index = FCString::Atoi(*Cmd[1]) + 1;
			auto investmentCandidate = InvestmentManagerRef->GetInvestmentCandidate();
			UInvestment* investment;
			if (investmentCandidate.Contains(index)) {
				investment = InvestmentManagerRef->GetInvestmentById(investmentCandidate[index]);
			} else {
				result += FString::Printf(TEXT("Command fail : Invalid index %d"), index);
			}
			if (investment->GetState() == InvestmentState::Processing) {
				investment->Success();
				result += FString::Printf(TEXT("Command success : Clear investment %d"), FCString::Atoi(*Cmd[1]));
			} else {
				result += FString::Printf(TEXT("Command fail : Investment %d is not started"), FCString::Atoi(*Cmd[1]));
			}
		} else {
			Success = false;
			result += TEXT("Invalid command : Not numeric input");
		}
		break;
	default: // Fail
		Success = false;
		result += TEXT("Invalid command : Not supported command");
	}
	return result;
}

// investment_fail : Fail all investment
// investment_fail {n} : Fail {n}th investment
FString AConsoleProcessor::CmdInvestmentFail(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Investment fail : ");
	switch (Cmd.Num()) {
	case 1: // investment_clear
		for (auto& i : InvestmentManagerRef->GetAllInvestment()) {
			if (i.Value->GetState() == InvestmentState::Processing) {
				i.Value->Fail();
			}
		}
		result += TEXT("Command success : Fail all investment");
		break;
	case 2: // investment_clear {n}
		if (Cmd[1].IsNumeric()) {
			int32 index = FCString::Atoi(*Cmd[1]) + 1;
			auto investmentCandidate = InvestmentManagerRef->GetInvestmentCandidate();
			UInvestment* investment;
			if (investmentCandidate.Contains(index)) {
				investment = InvestmentManagerRef->GetInvestmentById(investmentCandidate[index]);
			} else {
				result += FString::Printf(TEXT("Command fail : Invalid index %d"), index);
			}
			if (investment->GetState() == InvestmentState::Processing) {
				investment->Fail();
				result += FString::Printf(TEXT("Command success : Fail investment %d"), FCString::Atoi(*Cmd[1]));
			} else {
				result += FString::Printf(TEXT("Command fail : Investment %d is not started"), FCString::Atoi(*Cmd[1]));
			}
		} else {
			Success = false;
			result += TEXT("Invalid command : Not numeric input");
		}
		break;
	default: // Fail
		Success = false;
		result += TEXT("Invalid command : Not supported command");
	}
	return result;
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

// complain_on : Start increase complain all stations
// compalin_on {id} Start increase complain {id}'s station
FString AConsoleProcessor::CmdComplainOn(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Complain on : ");
	switch (Cmd.Num()) {
	case 1: // complain_on
		for (auto& i : StationManagerRef->GetAllStations()) {
			i->SetComplainIncreaseEnable(true);
		}
		result += TEXT("Command success : All station can increase complain");
		break;
	case 2: // complain_on {id}
		if (Cmd[1].IsNumeric()) {
			int32 stationId = FCString::Atoi(*Cmd[1]);
			auto stationRef = StationManagerRef->GetStationById(stationId);
			if (IsValid(stationRef)) {
				stationRef->SetComplainIncreaseEnable(true);
				result += FString::Printf(TEXT("Command success : Station id %d is can increase complain"), stationId);
			} else {
				Success = false;
				result += TEXT("Command fail : Invalid station");
			}
		} else {
			Success = false;
			result += TEXT("Invalid command : Not numeric input");
		}
		break;
	default: // Incorrect input
		Success = false;
		result += TEXT("Invalid command : Not supported command");
		break;
	}
	return result;
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
		result += TEXT("Command success : All station can not increase complain");
		break;
	case 2: // complain_off {id}
		if (Cmd[1].IsNumeric()) {
			int32 stationId = FCString::Atoi(*Cmd[1]);
			auto stationRef = StationManagerRef->GetStationById(stationId);
			if (IsValid(stationRef)) {
				stationRef->SetComplainIncreaseEnable(false);
				result += FString::Printf(TEXT("Command success : Station id %d is can not increase complain"), stationId);
			} else {
				Success = false;
				result += TEXT("Command fail : Invalid station");
			}
		} else {
			Success = false;
			result += TEXT("Invalid command : Not numeric input");
		}
		break;
	default: // Incorrect input
		Success = false;
		result += TEXT("Invalid command : Not supported command");
		break;
	}
	return result;
}

// complain_add {n} : Add {n} complain all station
// complain_add {n} {id} Add {n} complain {id}'s station
FString AConsoleProcessor::CmdComplainAdd(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Complain add : ");
	switch (Cmd.Num()) {
	case 2: // complain_add {n}
		if (Cmd[1].IsNumeric()) {
			for (auto& i : StationManagerRef->GetAllStations()) {
				i->AddComplain(FCString::Atof(*Cmd[1]));
			}
		} else {
			Success = false;
			result += TEXT("Incorrect input");
		}
		break;
	case 3: // complain_add {n} {id}
		if (Cmd[1].IsNumeric() && Cmd[2].IsNumeric()) {
			StationManagerRef->GetStationById(FCString::Atoi(*Cmd[2]))->AddComplain(FCString::Atof(*Cmd[1]));
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

// skip_time :  Set time next midnight
// skip_time {n} : Set time {n} day after midnight
FString AConsoleProcessor::CmdSkipTime(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Skip time : ");
	switch (Cmd.Num()) {
	case 1: // skip_time
		TimerRef->SkipTime();
		break;
	case 2: // skip_time {n}
		if (Cmd[1].IsNumeric()) {
			TimerRef->SkipTime(FCString::Atof(*Cmd[1]));
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

// add_passenger : Add random 1 passenger all station
// add_passenger {type} : Add {type} 1 passenger all station
// add_passenger {id} : Add random 1 passneger {id} station
// add_passenger {type} {id} : Add {type} 1 passenger {id} station
FString AConsoleProcessor::CmdAddPassenger(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Add passenger : ");
	StationType sType;
	switch (Cmd.Num()) {
	case 1: // add_passenger
		for (auto& i : StationManagerRef->GetAllStations()) {
			i->SpawnPassenger(StationManagerRef->GetRandomStationType());
		}
		result += TEXT("Add 1 passenger all stations");
		break;
	case 2: // add_passenger {type} or add_passenger {id}
		if (Cmd[1].IsNumeric()) { // add_passenger {id}
			StationManagerRef->GetStationById(FCString::Atoi(*Cmd[1]))->SpawnPassenger(StationManagerRef->GetRandomStationType());
			result += TEXT("Add 1 passenger on station");
		} else {
			sType = StationManagerRef->StationTypeFromString(Cmd[1], Success);
			if (Success) { // add_passenger {type}
				for (auto& i : StationManagerRef->GetAllStations()) {
					i->SpawnPassenger(sType);
				}
				result += TEXT("Add passenger all station");
			} else {
				Success = false;
				result += TEXT("Incorrect input");
			}
		}
		break;
	case 3: // add_passenger {type} {id}
		sType = StationManagerRef->StationTypeFromString(Cmd[1], Success);
		if (Success && Cmd[2].IsNumeric()) {
			StationManagerRef->GetStationById(FCString::Atoi(*Cmd[1]))->SpawnPassenger(sType);
			result += TEXT("Add passenger on station");
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

// del_passenger : Remove random passenger for all stations
// del_passenger {type} : Remove {type} passenger for all stations
// del_passenger {id} : Remove random passenger for {id} station
// del_passenger {type} {id} : Remove {type} passenger for {id} station
FString AConsoleProcessor::CmdDeletePassenger(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Add passenger : ");
	StationType sType;
	switch (Cmd.Num()) {
	case 1: // del_passenger
		for (auto& i : StationManagerRef->GetAllStations()) {
			i->DespawnRandomPassenger();
		}
		result += TEXT("Remove random passenger for all stations");
		break;
	case 2: // del_passenger {type} or del_passenger {id}
		if (Cmd[1].IsNumeric()) { // del_passenger {id}
			StationManagerRef->GetStationById(FCString::Atoi(*Cmd[1]))->DespawnRandomPassenger();
			result += TEXT("Remove random passenger for a station");
		} else {
			sType = StationManagerRef->StationTypeFromString(Cmd[1], Success);
			if (Success) { // del_passenger {type}
				for (auto& i : StationManagerRef->GetAllStations()) {
					i->DespawnPassenger(sType);
				}
				result += TEXT("Remove type passenger for all stations");
			} else {
				Success = false;
				result += TEXT("Incorrect input");
			}
		}
		break;
	case 3: // del_passenger {type} {id}
		sType = StationManagerRef->StationTypeFromString(Cmd[1], Success);
		if (Success && Cmd[2].IsNumeric()) {
			StationManagerRef->GetStationById(FCString::Atoi(*Cmd[1]))->DespawnPassenger(sType);
			result += TEXT("Remove type passenger for a station");
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

// passenger_generate : Show passenger spawn state (true or false)
// passenger_generate {true|false} : Set passenger spawn to {true|false}
FString AConsoleProcessor::CmdTogglePassengerSpawn(TArray<FString> Cmd, bool& Success) {
	FString result = TEXT("Passenger generate : ");
	bool flag;
	switch (Cmd.Num()) {
	case 1: // passenger_generate
		flag = StationManagerRef->GetPassengerSpawnEnable();
		result += TEXT("Passenger spawn enable = ");
		result += flag ? TEXT("true") : TEXT("false");
		break;
	case 2: // passenger_generate {true|false}
		if (Cmd[1] == TEXT("true")) {
			StationManagerRef->SetPassengerSpawnEnable(true);
			result += TEXT("Set true");
		} else if (Cmd[1] == TEXT("false")) {
			StationManagerRef->SetPassengerSpawnEnable(false);
			result += TEXT("Set false");
		} else {
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
			Result = CmdOccurEvent(splitStr, Success);
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
		} else if (splitStr[0] == TEXT("complain_on")) {
			Result = CmdComplainOn(splitStr, Success);
		} else if (splitStr[0] == TEXT("add_complain")) {
			Result = CmdComplainAdd(splitStr, Success);
		} else if (splitStr[0] == TEXT("skip_time")) {
			Result = CmdSkipTime(splitStr, Success);
		} else if (splitStr[0] == TEXT("add_passenger")) {
			Result = CmdAddPassenger(splitStr, Success);
		} else if (splitStr[0] == TEXT("del_passenger")) {
			Result = CmdDeletePassenger(splitStr, Success);
		} else if (splitStr[0] == TEXT("passenger_generate")) {
			Result = CmdTogglePassengerSpawn(splitStr, Success);
		}
	}

	Success = true;
	return Result;
}
