// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Subtrain.h"

ASubtrain::ASubtrain() {

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrainMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")
	);

	TrainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train Mesh"));
	TrainMeshComponent->SetWorldScale3D(FVector(2.5f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(TrainMesh.Object);
	//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	//TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);
}

void ASubtrain::Test() {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Subtrain"));
}

void ASubtrain::BeginPlay() {
	Super::BeginPlay();
}

FVector ASubtrain::GetNextTrainPosition() {
	return FVector();
}

void ASubtrain::SetOwnerTrainId(int32 TID) {
	OwnerTrainId = TID;
}

int32 ASubtrain::GetOwnerTrainId() const {
	return OwnerTrainId;
}
