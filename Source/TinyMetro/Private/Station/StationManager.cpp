// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AStationManager::AStationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStationManager::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle WaitHandle;
	float WaitTime = 0.2f;
	GetWorld()->GetTimerManager().SetTimer(
		WaitHandle, 
		this, 
		&AStationManager::SpawnStation, 
		1.0f, 
		true, 
		1.0f);
	
}

void AStationManager::TestFunction() {
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("Lambda")));
}

void AStationManager::SpawnStation() {
	StationSpawnCurrent += StationSpawnPerSec;
	if (StationSpawnCurrent >= StationSpawnRequire) {
		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Station/BP_Station.BP_Station'")));

		UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
		if (!SpawnActor) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
			return;
		}

		UClass* SpawnClass = SpawnActor->StaticClass();
		if (SpawnClass == NULL) {
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
			return;
		}

		UWorld* World = GetWorld();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		World->SpawnActor<AActor>(GeneratedBP->GeneratedClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.0f,
				FColor::Yellow,
				FString::Printf(TEXT("Spawn!")));
		StationSpawnCurrent = 0.0f;
	}

	//Log
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.0f,
			FColor::Yellow,
			FString::Printf(TEXT("%f"), StationSpawnCurrent));
}

// Called every frame
void AStationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1, 
			15.0f, 
			FColor::Yellow, 
			FString::Printf(TEXT("%d : %f"), GetWorld()->TimeSeconds, DeltaTime));*/

}

