// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/Connector/Connector.h"

// Sets default values
AConnector::AConnector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AConnector::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConnector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

