// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Station.h"

// Sets default values
AStation::AStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

