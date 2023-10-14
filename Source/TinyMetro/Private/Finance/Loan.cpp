// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Loan.h"
#include "Finance/LoanSaveGame.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManager.h"
#include <Kismet/GameplayStatics.h>

void ULoan::Repay() {
	if (RemainTime == 1) {
		RepayAll();
	} else {
		PlayerState->AddMoney(-(RepayPerWeek));
		StatisticsManagerRef->BankStatistics.TotalRepayMoney += RepayPerWeek;
		StatisticsManagerRef->BankStatistics.TotalRepayInterestMoney += RepayInterest;
		Balance -= RepayPerWeek;
		RemainTime--;
	}
}

void ULoan::NotifyWeeklyTask() {
	// Auto repay
	if (IsActivate) Repay();
}

void ULoan::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(WorldContextReference))->GetSaveManager();
	}
	ULoanSaveGame* tmp = Cast<ULoanSaveGame>(UGameplayStatics::CreateSaveGameObject(ULoanSaveGame::StaticClass()));

	tmp->LoanData = LoanData;
	tmp->Balance = Balance;
	tmp->RepayPerWeek = RepayPerWeek;
	tmp->RemainTime = RemainTime;
	tmp->IsActivate = IsActivate;
	tmp->IsAvailable = IsAvailable;

	SaveManagerRef->Save(tmp, SaveActorType::Loan, LoanData.Id);
}

void ULoan::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(WorldContextReference))->GetSaveManager();
	}
	ULoanSaveGame* tmp = Cast<ULoanSaveGame>(SaveManagerRef->Load(SaveActorType::Loan, LoanData.Id));

	if (!IsValid(tmp)) {
		return;
	}

	LoanData = tmp->LoanData;
	Balance = tmp->Balance;
	RepayPerWeek = tmp->RepayPerWeek;
	RemainTime = tmp->RemainTime;
	IsActivate = tmp->IsActivate;
	IsAvailable = tmp->IsAvailable;
}

void ULoan::RepayAll() {
	PlayerState->AddMoney(-(Balance));
	StatisticsManagerRef->BankStatistics.TotalRepayMoney += Balance;
	StatisticsManagerRef->BankStatistics.TotalRepayInterestMoney += RepayInterest * RemainTime;
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

void ULoan::SetPlayerState(ATinyMetroPlayerState* P) {
	PlayerState = P;
}

void ULoan::SetWorld(UWorld* W) {
	WorldContextReference = W;
}

void ULoan::SetStatisticsManager(AStatisticsManager* S) {
	StatisticsManagerRef = S;
}

void ULoan::SetSaveManager(ATMSaveManager* S) {
	SaveManagerRef = S;
}

void ULoan::SetAvailabilityFunction(TFunction<bool(void)> Func) {
	CheckAvailable = Func;
}

void ULoan::InitLoan(FLoanData Data) {
	Balance = Data.Amount * FMath::Pow(1 + (Data.Rate / 52), Data.DueDateWeek);
	RepayInterest = Balance / Data.DueDateWeek;
	RepayPerWeek = Balance / Data.DueDateWeek;
	RemainTime = Data.DueDateWeek;
}
