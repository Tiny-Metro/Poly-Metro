// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/SampleTimer.h"


SampleTimer::SampleTimer(UWorld* World, FLoanData& Data) : LoanData(Data) {
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.0f,
	//		FColor::Yellow,
	//		FString::Printf(TEXT("SmapleTimer : %d"), Data.Amount));

	this->World = World;
	Test();
}

SampleTimer::~SampleTimer(){
}

void SampleTimer::Test() {
	World->GetTimerManager().SetTimer(
		TestHandle,
		FTimerDelegate::CreateLambda([LoanData = LoanData]() {
			//Log
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.0f,
					FColor::Yellow,
					FString::Printf(TEXT("SmapleTimer : %d"), LoanData.Amount));
		}),
		1.0f,
		true,
		0.0f
	);
}
