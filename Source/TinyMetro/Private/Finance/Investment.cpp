// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Investment.h"

void UInvestment::SetInvestmentData(FInvestmentData Data) {
	InvestmentData = Data;
}

void UInvestment::SetDaytime(int32 T) {
	Daytime = T;
}

void UInvestment::SetPlayerState(ATinyMetroPlayerState* P) {
	PlayerState = P;
}

void UInvestment::SetWorld(UWorld* W) {
	World = W;
}

void UInvestment::SetSuccessFunction(TFunction<bool(void)> Func) {
	CheckSuccess = Func;
}

void UInvestment::InvestmentSuccess() {
	
	InitInvestment();
}

void UInvestment::InvestmentFail() {

	InitInvestment();
}

void UInvestment::InitInvestment() {
	World->GetTimerManager().ClearTimer(InvestmentTimeHandle);
	World->GetTimerManager().ClearTimer(InvestmentSuccessHandle);
	IsActivate = false;
	RemainTime = InvestmentData.TimeRequire;
}

void UInvestment::ActivateInvestment() {
	IsActivate = true;
	World->GetTimerManager().SetTimer(
		InvestmentTimeHandle,
		FTimerDelegate::CreateLambda([&RemainTime = RemainTime, this]() {
			if (RemainTime == 0) {
				this->InvestmentFail();
			} else {
				RemainTime--;
			}
			

			}),
		Daytime,
		true,
		Daytime - (FMath::Fmod(World->GetTimeSeconds(), Daytime))
	);

	World->GetTimerManager().SetTimer(
		InvestmentSuccessHandle,
		FTimerDelegate::CreateLambda([&CheckSuccess = CheckSuccess, this]() {
			if (CheckSuccess()) {
				this->InvestmentSuccess();
			}
		}),
		0.3f,
		true,
		0.0f
	);
}

FInvestmentData UInvestment::GetInvestmentData() const {
	return InvestmentData;
}

bool UInvestment::GetIsActivate() const {
	return IsActivate;
}
