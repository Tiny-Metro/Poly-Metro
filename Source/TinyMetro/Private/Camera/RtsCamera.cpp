// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/RtsCamera.h"

// Sets default values
ARtsCamera::ARtsCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARtsCamera::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARtsCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ARtsCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

