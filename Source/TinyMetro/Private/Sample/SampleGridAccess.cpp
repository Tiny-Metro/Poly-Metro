// Fill out your copyright notice in the Description page of Project Settings.


#include "Sample/SampleGridAccess.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ASampleGridAccess::ASampleGridAccess()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASampleGridAccess::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASampleGridAccess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

