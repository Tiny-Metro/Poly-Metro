// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Subtrain.h"

ASubtrain::ASubtrain() {

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainShort.SM_SubtrainShort'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);

	TrainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train Mesh"));
	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(LoadTrainMesh.Object);
	//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	//TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);
}

void ASubtrain::Test() {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Subtrain"));
}

void ASubtrain::SetTrainMaterial(ALane* Lane) {
}

void ASubtrain::BeginPlay() {
	Super::BeginPlay();
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
