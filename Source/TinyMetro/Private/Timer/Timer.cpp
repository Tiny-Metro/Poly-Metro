// Fill out your copyright notice in the Description page of Project Settings.


#include "Timer/Timer.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Station/StationManager.h"
#include "Timer/TimerSaveGame.h"
#include "SaveSystem/TMSaveManager.h"
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
	if(!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	Daytime = GameModeRef->GetDaytime();

	Load();

	const UEnum* enumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("Day"), true);
	FString dayName = enumPtr->GetNameStringByValue((uint8)Timestamp.DayoftheWeek);
	GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Black, dayName);

	SaveManagerRef->SaveTask.AddDynamic(this, &ATimer::Save);
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
		Timestamp.DayoftheWeek = static_cast<Day>(Timestamp.Date % 7);
		Timestamp.Date++;
		DayCounter -= Daytime;

		DailyTask.Broadcast();
	}

	// Weekly check
	if (WeekCounter >= (Daytime * 7)) {
		Timestamp.Week++;
		WeekCounter -= (Daytime * 7);

		WeeklyTask.Broadcast();
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

void ATimer::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UTimerSaveGame* tmp = Cast<UTimerSaveGame>(UGameplayStatics::CreateSaveGameObject(UTimerSaveGame::StaticClass()));

	tmp->ElapseTimeSec = ElapseTimeSec;
	tmp->Timestamp = Timestamp;
	tmp->DayCounter = DayCounter;
	tmp->WeekCounter = WeekCounter;
	tmp->SkiptimeTarget = SkiptimeTarget;
	tmp->SkiptimeFlag = SkiptimeFlag;

	SaveManagerRef->Save(tmp, SaveActorType::Timer);
}

void ATimer::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UTimerSaveGame* tmp = Cast<UTimerSaveGame>(SaveManagerRef->Load(SaveActorType::Timer));

	if (!IsValid(tmp)) {
		return;
	}

	ElapseTimeSec = tmp->ElapseTimeSec;
	Timestamp = tmp->Timestamp;
	DayCounter = tmp->DayCounter;
	WeekCounter = tmp->WeekCounter;
	SkiptimeTarget = tmp->SkiptimeTarget;
	SkiptimeFlag = tmp->SkiptimeFlag;
}

