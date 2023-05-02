// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer/Timer.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATimer::ATimer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATimer::BeginPlay()
{
	Super::BeginPlay();

	// Get GameMode, set daytime
	GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	StationManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();
	Daytime = GameModeRef->GetDaytime();
}

// Called every frame
void ATimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ElapseTimeSec += DeltaTime;
	DayCounter += DeltaTime;
	WeekCounter += DeltaTime;

	// Daily check
	if (DayCounter >= Daytime) {
		StationManagerRef->DailyTask();
		DayCounter -= Daytime;
	}

	// Weekly check
	if (WeekCounter >= (Daytime * 7)) {
		StationManagerRef->WeeklyTask();
		WeekCounter -= (Daytime * 7);
	}
}

