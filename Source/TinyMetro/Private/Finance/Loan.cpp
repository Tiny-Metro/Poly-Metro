// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Loan.h"
#include "./GameModes/TinyMetroGameModeBase.h"

void ULoan::Test() {

}

void ULoan::Repay() {
}

void ULoan::CalculateInterest() {
}

FLoanData ULoan::GetLoanData() const {
	return LoanData;
}

void ULoan::SetLoanData(FLoanData Data) {
	this->LoanData = Data;
}

void ULoan::SetDaytime() {
	ATinyMetroGameModeBase* GameMode = (ATinyMetroGameModeBase*)GetWorld()->GetAuthGameMode();
	Daytime = GameMode->GetDaytime();
}
