// Fill out your copyright notice in the Description page of Project Settings.


#include "MycameraActor.h"

// Sets default values
AMycameraActor::AMycameraActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMycameraActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMycameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	Timer += DeltaTime;

	FVector NewLocation = GetActorLocation();
	NewLocation.Y += FMath::Cos(Timer);
	SetActorLocation(NewLocation);

}

