// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationSpawnPulse.h"

// Sets default values
AStationSpawnPulse::AStationSpawnPulse()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot")));

	// Set station pulse effect mesh
	PulseComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pulse plane"));
	PulseComponent->SetupAttachment(RootComponent);
	PulseComponent->SetGenerateOverlapEvents(false);
	//PulseComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Plane.Plane'")).Object);
	//PulseComponent->SetMaterial(0, ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Station/Asset/StationMaterial/M_RingPulse.M_RingPulse'")).Object);
	PulseComponent->SetWorldScale3D(FVector(20.0f, 20.0f, 1.0f));
}

// Called when the game starts or when spawned
void AStationSpawnPulse::BeginPlay()
{
	Super::BeginPlay();
	
}

