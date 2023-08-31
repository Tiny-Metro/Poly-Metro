// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/TinyMetroEventManager.h"
#include "Event/TinyMetroEvent.h"
#include "SaveSystem/TMSaveManager.h"
#include "Timer/Timer.h"
#include "Lua/EventLuaState.h"
#include <Kismet/GameplayStatics.h>
#include "GameModes/TinyMetroGameModeBase.h"

// Sets default values
ATinyMetroEventManager::ATinyMetroEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATinyMetroEventManager::BeginPlay()
{
	Super::BeginPlay();
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(LuaState)) LuaState = UEventLuaState::CreateInstance(GetWorld());

	// Read lua and make Event
	// Call Load function
	// - Load fail(new game) : Read Content/Script/Event & Copy them to Content/Script/Event/Saved
	// - Load success(saved game) : Read Content/Script/Event/Saved
	InitializeEvent();


	TimerRef->DailyTask.AddDynamic(this, &ATinyMetroEventManager::DailyTask);
	
	SaveManagerRef->SaveTask.AddDynamic(this, &ATinyMetroEventManager::Save);
}

bool ATinyMetroEventManager::OccurSpecificEvent(int32 EventId) {
	// Check valid id
	if (EventArr.Find(EventId) != nullptr) {
		// Fill up only 1 event (EventId)
		EventCandidate.Empty();
		EventCandidate.Add(EventId);
		// Occur event
		OccurEvent();
		return true;
	} else {
		return false;
	}
}

void ATinyMetroEventManager::DailyTask() {
	UE_LOG(LogTemp, Log, TEXT("EventManager::DailyTask"));

	// Update event counter
	EventPeriod--;
	if (EventPeriod >= 0) {
		Day++;
		EventEndTask.Broadcast();
	}

	SimulateProbability();
}

void ATinyMetroEventManager::Save() {
}

bool ATinyMetroEventManager::Load() {

	return false;
}

void ATinyMetroEventManager::SimulateProbability() {
	float EventProbability = (Day > (uint32)EventOccurProbability.Num()) ? 1.0 : EventOccurProbability[Day];
	//float EventProbability = 1.0f;
	if (EventProbability > FMath::RandRange(0.0, 1.0)) {
		UpdateEventWeight();
		OccurEvent();
		Day = 0;
	}
}

void ATinyMetroEventManager::OccurEvent() {
	UE_LOG(LogTemp, Log, TEXT("EventManager::OccurEvent"));
	int32 occurEventIdx = FMath::RandRange(0, EventCandidate.Num() - 1);

	SelectedEvent = (*EventArr.Find(EventCandidate[occurEventIdx]));
	if (SelectedEvent != nullptr) {
		EventPeriod = SelectedEvent->EventStart();
		FTinyMetroEventOccurData eventLog;
		eventLog.EventData = SelectedEvent->GetEventData();
		eventLog.Timestamp = TimerRef->GetTimestamp();
		EventOccurLog.Add(eventLog);
		EventOccurTask.Broadcast();
	}
}

void ATinyMetroEventManager::UpdateEventWeight() {
	// Clear event wiehgt list
	EventCandidate.Empty();

	// Fill up event IDs by weight
	for (int i = 0; i < EventArr.Num() - 1; i++) {
		int eventWeight = EventArr[i]->GetEventWeight();
		for (int j = 0; j < eventWeight; j++) EventCandidate.Add(i);
	}

	for (auto& i : EventArr) {
		int eventWeight = i.Value->GetEventWeight();
		for (int j = 0; j < eventWeight; j++) EventCandidate.Add(i.Key);
	}

}

void ATinyMetroEventManager::InitializeEvent() {
	TArray<FString> findFiles;
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	FRegexPattern pattern(TEXT("Event\\.[1-9]\\d{3}\\.lua"));
	FString scriptDir = FPaths::ProjectContentDir()
		.Append(TEXT("Script"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Event"));

	if (!Load()) {
		// Load fail : New game
		// Copy files to Content/Script/Event/Saved
		platformFile.FindFiles(findFiles, *scriptDir, TEXT("lua"));
		for (auto& i : findFiles) {
			FRegexMatcher matcher(pattern, i);
			if (matcher.FindNext()) {
				FString saveEventDir = FPaths::GetPath(i)
					.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
					.Append(TEXT("Saved"))
					.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
				FString cleanFileName = FPaths::GetCleanFilename(i);

				if (!platformFile.DirectoryExists(*saveEventDir)) platformFile.CreateDirectory(*saveEventDir);

				platformFile.CopyFile(*(saveEventDir + cleanFileName), *i);
			}
		}
	}

	scriptDir
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Saved"));

	findFiles.Empty();
	platformFile.FindFiles(findFiles, *scriptDir, TEXT("lua"));

	// Read files
	// Read directory = saved directory
	for (auto& i : findFiles) {
		FRegexMatcher matcher(pattern, i);
		if (matcher.FindNext()) {
			auto tmpEvent = UTinyMetroEvent::CreateEvent(FPaths::GetCleanFilename(i), LuaState, GetWorld());
			EventArr.Add(tmpEvent->GetEventData().Id, tmpEvent);
		}
	}

}

// Called every frame
void ATinyMetroEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

