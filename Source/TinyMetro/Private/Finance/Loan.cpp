// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Loan.h"
#include <Kismet/GameplayStatics.h>

void ULoan::Repay() {
	if (Remainder == 1) {
		RepayAll();
	} else {
		PlayerState->AddMoney(-(RepayPerWeek));
		Balance -= RepayPerWeek;
		Remainder--;
	}
}

void ULoan::RepayAll() {
	PlayerState->AddMoney(-(Balance));
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
	World->GetTimerManager().SetTimer(
		LoanHandle,
		FTimerDelegate::CreateLambda([LoanData = LoanData, this]() {
			// Auto repay logic
			this->Repay();

			////Log
			//if (GEngine)
			//	GEngine->AddOnScreenDebugMessage(
			//		-1,
			//		15.0f,
			//		FColor::Yellow,
			//		FString::Printf(TEXT("SampleTimer : %d"), LoanData.Amount));
			}),
		Daytime * 7,
		true,
		(Daytime * 7) - (FMath::Fmod(World->GetTimeSeconds(), Daytime * 7))
		);

	////Log
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.0f,
	//		FColor::Yellow,
	//		FString::Printf(TEXT("Total : %f"), FMath::Fmod(3.3, 2.2)));

}

void ULoan::DisableLoan() {
	World->GetTimerManager().ClearTimer(LoanHandle);
	IsActivate = false;
	InitLoan(LoanData);

	////Log
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.0f,
	//		FColor::Yellow,
	//		FString::Printf(TEXT("Disable Loan : %d"), 0));
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
	World = W;
}

void ULoan::SetAvailabilityFunction(TFunction<bool(void)> Func) {
	CheckAvailable = Func;
}

void ULoan::InitLoan(FLoanData Data) {
	Balance = Data.Amount * FMath::Pow(1 + (Data.Rate / 52), Data.DueDateWeek);
	RepayPerWeek = Balance / Data.DueDateWeek;
	Remainder = Data.DueDateWeek;
}
