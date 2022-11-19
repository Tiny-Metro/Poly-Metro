// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraMyActor.h"

// Sets default values
ACameraMyActor::ACameraMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACameraMyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACameraMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Fvector NewLocation = GetActorLocation();
	NewLocation.Y += FMath::Cos(Timer);
	SetActorLocation(NewLocation);
}

