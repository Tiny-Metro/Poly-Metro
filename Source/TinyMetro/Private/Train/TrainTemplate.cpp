// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainTemplate.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
ATrainTemplate::ATrainTemplate()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	auto TrainMovement = GetCharacterMovement();
	TrainMovement->MaxAcceleration = 50000.0f; // Default 2048
	TrainMovement->Mass = 1.0f; // Deafult 100
	TrainMovement->MaxWalkSpeed = 300.0f; // Default 600
	TrainMovement->bUseControllerDesiredRotation = true; // Default false
}

// Called when the game starts or when spawned
void ATrainTemplate::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		TEXT("Train Spawn"));
	
}

// Called every frame
void ATrainTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrainTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATrainTemplate::SetTrainId(int32 Id) {
	TrainId = Id;
}

int32 ATrainTemplate::GetTrainId() const {
	return TrainId;
}

void ATrainTemplate::SetServiceLaneId(int32 Id) {
	ServiceLaneId = Id;
}

int32 ATrainTemplate::GetServiceLaneId() const {
	return ServiceLaneId;
}

void ATrainTemplate::SetTrainDirection(TrainDirection Dir) {
	Direction = Dir;
}

TrainDirection ATrainTemplate::GetTrainDirection() const {
	return Direction;
}

void ATrainTemplate::Upgrade() {
	IsUpgrade = true;

	//TODO : Mesh change
}

void ATrainTemplate::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : TrainTemplate"));
}

FVector ATrainTemplate::GetNextTrainPosition() {
	return FVector();
}

