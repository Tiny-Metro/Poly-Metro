// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/BridgeTunnel/BridgeTunnel.h"

// Sets default values
ABridgeTunnel::ABridgeTunnel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABridgeTunnel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABridgeTunnel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

