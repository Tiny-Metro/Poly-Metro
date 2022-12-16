// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Loan.h"
#include <Kismet/GameplayStatics.h>

void ULoan::Test() {

}

void ULoan::Repay() {
}

void ULoan::CalculateInterest() {
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
		FTimerDelegate::CreateLambda([LoanData = LoanData]() {
			// Interest logic

			// Auto repay logic

			//Log
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.0f,
					FColor::Yellow,
					FString::Printf(TEXT("SmapleTimer : %d"), LoanData.Amount));
			}),
		10.0f,
		true,
		0.0f
		);


}

void ULoan::SetLoanData(FLoanData Data) {
	this->LoanData = Data;
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
