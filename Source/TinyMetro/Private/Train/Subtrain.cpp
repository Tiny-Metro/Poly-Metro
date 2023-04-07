// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Subtrain.h"
#include "Train/SubtrainAiController.h"
#include <GameFramework/CharacterMovementComponent.h>

ASubtrain::ASubtrain() {

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainShort.SM_SubtrainShort'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);

	TrainMovement->MaxWalkSpeed = 400.0f; // Default 600

	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(LoadTrainMesh.Object);
	//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	//TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);

	UpdatePassengerSlot();
}

void ASubtrain::Test() {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Subtrain"));
}

void ASubtrain::SetTrainMaterial(ALane* Lane) {
	Super::SetTrainMaterial(Lane);
}

void ASubtrain::UpdatePassengerSlot() {
	Super::UpdatePassengerSlot();
	if (IsUpgrade) {
		for (int i = 0; i < PassengerMeshPositionUpgrade.Num(); i++) {
			PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPositionUpgrade[i]);
		}
	} else {
		for (int i = 0; i < PassengerMeshPosition.Num(); i++) {
			PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPosition[i]);
		}
	}
}

void ASubtrain::BeginPlay() {
	Super::BeginPlay();
	AiControllerRef = Cast<ASubtrainAiController>(GetController());
}

//FVector ASubtrain::GetNextTrainPosition() {
//	return FVector();
//}

void ASubtrain::SetOwnerTrainId(int32 TID) {
	OwnerTrainId = TID;
}

int32 ASubtrain::GetOwnerTrainId() const {
	return OwnerTrainId;
}

void ASubtrain::SetDistanceFromTrain(float Dist) {
	DistanceFromTrain = Dist;
	AiControllerRef->SetDistanceFromTrain(Dist);
}
