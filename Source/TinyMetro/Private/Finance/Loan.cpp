// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Loan.h"
#include "Statistics/StatisticsManager.h"
#include <Kismet/GameplayStatics.h>

void ULoan::Repay() {
	if (RemainTime == 1) {
		RepayAll();
	} else {
		PlayerState->AddMoney(-(RepayPerWeek));
		StatisticsManagerRef->BankStatistics.TotalRepayMoney += RepayPerWeek;
		Balance -= RepayPerWeek;
		RemainTime--;
	}
}

void ULoan::NotifyWeeklyTask() {
	// Auto repay
	if (IsActivate) Repay();
}

void ULoan::RepayAll() {
	PlayerState->AddMoney(-(Balance));
	StatisticsManagerRef->BankStatistics.TotalRepayMoney += Balance;
	DisableLoan();
}

FLoanData ULoan::GetLoanData() const {
	return LoanData;
}

bool ULoan::GetIsActivate() const {
	return IsActivate;
}

bool ULoan::GetAvailable() {
	IsAvailable = CheckAvailable();
	return IsAvailable;
}

void ULoan::ActivateLoan() {
	IsActivate = true;
	PlayerState->AddMoney(LoanData.Amount);
	StatisticsManagerRef->BankStatistics.TotalUsingLoanCount++;
	StatisticsManagerRef->BankStatistics.TotalLoanMoney += LoanData.Amount;
}

void ULoan::DisableLoan() {
	IsActivate = false;
	InitLoan(LoanData);
}

void ULoan::SetLoanData(FLoanData Data) {
	this->LoanData = Data;
	InitLoan(Data);
}

void ULoan::SetDaytime(int32 T) {
	Daytime = T;
}

void ULoan::SetPlayerState(ATinyMetroPlayerState* P) {
	PlayerState = P;
}

void ULoan::SetWorld(UWorld* W) {
	WorldContextReference = W;
}

void ULoan::SetStatisticsManager(AStatisticsManager* S) {
	StatisticsManagerRef = S;
}

void ULoan::SetAvailabilityFunction(TFunction<bool(void)> Func) {
	CheckAvailable = Func;
}

void ULoan::InitLoan(FLoanData Data) {
	Balance = Data.Amount * FMath::Pow(1 + (Data.Rate / 52), Data.DueDateWeek);
	RepayPerWeek = Balance / Data.DueDateWeek;
	RemainTime = Data.DueDateWeek;
}
