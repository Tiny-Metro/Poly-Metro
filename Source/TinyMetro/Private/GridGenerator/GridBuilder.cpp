// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGenerator/GridBuilder.h"

// Sets default values
AGridBuilder::AGridBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGridBuilder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGridBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

