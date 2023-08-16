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
	//StationManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();
	Daytime = GameModeRef->GetDaytime();

	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("Day"), true);
	FString dayName = enumPtr->GetNameStringByValue((uint8)Timestamp.DayoftheWeek);
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Black, dayName);
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
		DailyTask.Broadcast();

		Timestamp.DayoftheWeek = static_cast<Day>(Timestamp.Date % 7);
		Timestamp.Date++;

		DayCounter -= Daytime;
	}

	// Weekly check
	if (WeekCounter >= (Daytime * 7)) {
		WeeklyTask.Broadcast();
		Timestamp.Week++;
		WeekCounter -= (Daytime * 7);
	}

	if (SkiptimeTarget <= ElapseTimeSec && SkiptimeFlag) {
		GameModeRef->SetGameSpeed(1.0f);
		SkiptimeFlag = false;
	}
}

void ATimer::SkipTime(int32 Skipday) {
	SkiptimeTarget = ((int32)(ElapseTimeSec / Daytime) + Skipday) * Daytime; // Next day time
	SkiptimeFlag = true;
	GameModeRef->SetGameSpeed(1000.0f);
}

FTimestamp ATimer::GetTimestamp() {
	return Timestamp;
}

int32 ATimer::GetDaytime() const {
	return Daytime;
}

