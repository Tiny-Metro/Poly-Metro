// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/SampleGameMode.h"

void ASampleGameMode::BeginPlay() {
	GetWorldSettings()->SetTimeDilation(1.0f);
}

FString ASampleGameMode::GetFileName() const {
	return TEXT("map.txt");
}
