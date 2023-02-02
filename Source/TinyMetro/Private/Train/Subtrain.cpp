// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Subtrain.h"

void ASubtrain::Test() {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Subtrain"));
}

FVector ASubtrain::GetNextTrainPosition() {
	return FVector();
}
