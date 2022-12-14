// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/SampleTimer.h"


SampleTimer::SampleTimer(UWorld* World, int32 Amount, int32 DueDate, float Rate) {
	this->Amount = Amount;
	this->DueDate = DueDate;
	this->Rate = Rate;
	this->World = World;
	Test();
}

SampleTimer::~SampleTimer(){
}

void SampleTimer::Test() {
	World->GetTimerManager().SetTimer(
		TestHandle,
		FTimerDelegate::CreateLambda([&]() {
			//Log
			if (GEngine)
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.0f,
					FColor::Yellow,
					FString::Printf(TEXT("SmapleTimer : %d"), Amount));
		}),
		1.0f,
		true,
		0.0f
	);
}
