// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainManager.h"

// Sets default values
ATrainManager::ATrainManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrainManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

