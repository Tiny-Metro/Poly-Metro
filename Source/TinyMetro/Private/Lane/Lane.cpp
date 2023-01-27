// Fill out your copyright notice in the Description page of Project Settings.


#include "Lane/Lane.h"

// Sets default values
ALane::ALane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

