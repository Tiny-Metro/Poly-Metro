// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Subtrain.h"
#include "Train/Train.h"
#include "Train/TrainManager.h"
#include "Train/SubtrainAiController.h"
#include "Lane/Lane.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetMathLibrary.h>

ASubtrain::ASubtrain() {

	static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainShort.SM_SubtrainShort'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);

	TrainMovement->MaxWalkSpeed = 350.0f; // Default 600
	TrainMovement->MaxAcceleration = 3000.0f; // Default 2048

	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(LoadTrainMesh.Object);
	//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	//TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);

	UpdatePassengerSlot();
}

void ASubtrain::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Drag activated
	if (IsActorDragged) {
		DropPassenger();
		DetachFromTrain();
		FVector MouseToWorldLocation;
		AActor* MouseToWorldActor = ConvertMousePositionToWorldLocation(MouseToWorldLocation);

		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Train::Tick - %lf, %lf"), MouseToWorldLocation.X, MouseToWorldLocation.Y));*/
		LaneRef = Cast<ALane>(MouseToWorldActor);
		SetTrainMaterial(LaneRef);
		if (MouseToWorldActor->IsA(AStation::StaticClass())) {
			LineTraceIgnoreActors.AddUnique(MouseToWorldActor);
		}
		if (IsValid(LaneRef)) {

		}

		TrainRef = TrainManagerRef->GetNearestTrain(MouseToWorldLocation, LaneRef);
		if (IsValid(TrainRef)) {
			this->SetActorLocationAndRotation(
				MouseToWorldLocation,
				FRotator(
					0,
					UKismetMathLibrary::FindLookAtRotation(MouseToWorldLocation, TrainRef->GetActorLocation()).Yaw,
					0
				)
			);
		}
	}
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

void ASubtrain::DespawnTrain() {
	PlayerStateRef->AddItem(ItemType::Subtrain, 1);
	Super::DespawnTrain();
}

void ASubtrain::TrainOnReleased(AActor* Target, FKey ButtonPressed) {
	Super::TrainOnReleased(Target, ButtonPressed);
	if (IsValid(LaneRef)) {
		FVector StartLocation = GridManagerRef->Approximate(GetActorLocation(), LaneRef->GetLaneShape(GetActorLocation()));
		StartLocation.Z = 20.0f;
		SetActorLocation(StartLocation);
		TrainRef->AddSubtrain(this);
		ServiceStart(GetActorLocation(), LaneRef, nullptr);
	} else {
		DespawnTrain();
	}
	
}

void ASubtrain::BeginPlay() {
	Super::BeginPlay();
	AiControllerRef = Cast<ASubtrainAiController>(GetController());
}

//FVector ASubtrain::GetNextTrainPosition() {
//	return FVector();
//}

void ASubtrain::ServiceStart(FVector StartLocation, ALane* Lane, AStation* D) {
	Super::ServiceStart(StartLocation, Lane, D);
}

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

void ASubtrain::DetachFromTrain() {
	IsAttached = false;
	if (IsValid(OwnerTrain)) {
		OwnerTrain->DetachSubtrain(this);
	}
	OwnerTrain = nullptr;
	OwnerTrainId = -1;
}

void ASubtrain::AttachToTrain(ATrain* Train) {
	IsAttached = true;
	OwnerTrain = Train;
	OwnerTrainId = Train->GetTrainId();
}
