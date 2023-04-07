// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/SubtrainAiController.h"

void ASubtrainAiController::Follow_Implementation() {
}

void ASubtrainAiController::SetTargetTrain(ATrainTemplate* T) {
	TargetTrain = T;
}

void ASubtrainAiController::SetDistanceFromTrain(float Dist) {
	DistanceFromTrain = Dist;
}
