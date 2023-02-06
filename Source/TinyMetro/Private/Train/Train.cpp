// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"
#include "Train/SubtrainAiController.h"

void ATrain::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Train"));
}

void ATrain::BeginPlay() {
	Super::BeginPlay();
	TotalTravel = 0.0f;
	ParentAiControllerRef = Cast<ATrainAiController>(GetController());

	if (IsValid(ParentAiControllerRef)) {
		UE_LOG(LogTemp, Log, TEXT("Success"));
	}

}

//void ATrain::Tick(float DeltaTime) { }

FVector ATrain::GetNextTrainPosition() {
	return FVector();
}

void ATrain::SetSubtrain(ASubtrain* T) {
	Cast<ASubtrainAiController>(T->GetController())->SetTargetTrain(this);
	Subtrains.Add(T);
}

