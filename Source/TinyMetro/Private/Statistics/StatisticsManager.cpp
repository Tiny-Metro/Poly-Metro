// Fill out your copyright notice in the Description page of Project Settings.


#include "Statistics/StatisticsManager.h"

// Sets default values
AStatisticsManager::AStatisticsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStatisticsManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStatisticsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

