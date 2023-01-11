// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainTemplate.h"

// Sets default values
ATrainTemplate::ATrainTemplate()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainTemplate::BeginPlay()
{
	Super::BeginPlay();
	
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

