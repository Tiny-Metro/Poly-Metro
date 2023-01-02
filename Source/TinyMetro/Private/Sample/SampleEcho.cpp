// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/SampleEcho.h"

// Sets default values
ASampleEcho::ASampleEcho()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASampleEcho::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASampleEcho::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

