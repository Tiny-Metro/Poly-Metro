// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"

void ATrain::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Train"));
}

void ATrain::BeginPlay() {
	TotalTravel = 0.0f;
}

void ATrain::Tick(float DeltaTime) {
}

FVector ATrain::GetTrainNextLocation() {
	return FVector();
}

