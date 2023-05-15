// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Investment.h"

UInvestment* UInvestment::CreateInvestment(FInvestmentData Data, int32 Daytime, TFunction<void(void)> Start, TFunction<void(void)> Success, TFunction<void(void)> Fail, TFunction<InvestmentState(void)> Check){
	UInvestment* Obj = NewObject<UInvestment>();
	Obj->SetInvestmentData(Data);
	Obj->SetDaytime(Daytime);
	Obj->SetAcceptAction(Start);
	Obj->SetSuccessAction(Success);
	Obj->SetFailAction(Fail);
	Obj->SetStateCheckFunction(Check);
	
	return Obj;
}

void UInvestment::SetDaytime(int32 T) {
	Daytime = T;
}

void UInvestment::SetInvestmentData(FInvestmentData Data) {
	InvestmentData = Data;
}

void UInvestment::SetAcceptAction(TFunction<void(void)> Action) {
	AcceptAction = Action;
}

void UInvestment::SetSuccessAction(TFunction<void(void)> Action) {
	SuccessAction = Action;
}

void UInvestment::SetFailAction(TFunction<void(void)> Action) {
	FailAction = Action;
}

void UInvestment::SetStateCheckFunction(TFunction<InvestmentState(void)> Check) {
	ConditionCheckFunction = Check;
}

void UInvestment::InvestmentStart() {
	State = InvestmentState::Processing;
}

void UInvestment::InitInvestment() {
	State = InvestmentState::Ready;
	RemainTime = InvestmentData.TimeRequire;
}

InvestmentState UInvestment::CheckInvestmentProcess(float DeltaTime) {
	if (State == InvestmentState::Processing) {
		switch (ConditionCheckFunction()) {
		case InvestmentState::Fail:
			FailAction();
			return InvestmentState::Fail;
		case InvestmentState::Success:
			if (ElapseTime >= Daytime * InvestmentData.TimeRequire) { // Time over : Success
				SuccessAction();
				return InvestmentState::Success;
			} else { // Process investment
				return InvestmentState::Processing;
			}
		default:
			return InvestmentState::Processing;
		}
	}

	return InvestmentState::Ready;
}

void UInvestment::NotifyDailyTask() {
	if (State == InvestmentState::Processing) {
		ElapseTime += Daytime;
	}
}

FInvestmentData UInvestment::GetInvestmentData() const {
	return InvestmentData;
}

InvestmentState UInvestment::GetState() const {
	return State;
}
