// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Investment.h"

void UInvestment::SetInvestmentData(FInvestmentData Data) {
	InvestmentData = Data;
}

void UInvestment::SetDaytime(int32 T) {
	Daytime = T;
}

void UInvestment::SetPlyaerState(ATinyMetroPlayerState* P) {
	PlayerState = P;
}

void UInvestment::SetWorld(UWorld* W) {
	World = W;
}

void UInvestment::InvestmentSuccess() {
}

void UInvestment::InvestmentFail() {
}

FInvestmentData UInvestment::GetInvestmentData() const {
	return InvestmentData;
}

bool UInvestment::GetIsActivate() const {
	return IsActivate;
}

void UInvestment::ActivateInvestment() {
}
