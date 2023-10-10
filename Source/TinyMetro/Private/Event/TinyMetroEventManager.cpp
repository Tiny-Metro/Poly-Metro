// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/TinyMetroEventManager.h"
#include "Event/TinyMetroEvent.h"
#include "Event/TinyMetroEventManagerSaveGame.h"
#include "Sound/SoundManager.h"
#include "SaveSystem/TMSaveManager.h"
#include "Timer/Timer.h"
#include "Lua/EventLuaState.h"
#include "LuaMachine/Public/LuaComponent.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
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
	if (!IsValid(SoundManagerRef)) SoundManagerRef = GameModeRef->GetSoundManager();

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
	if (LuaComponentArr.Find(EventId) != nullptr) {
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
	ExpirationDay--;
	if (ExpirationDay >= 0) {
		ProgressDay++;
		EventEndTask.Broadcast();
	}

	SimulateProbability();
}

void ATinyMetroEventManager::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UTinyMetroEventManagerSaveGame* tmp = Cast<UTinyMetroEventManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UTinyMetroEventManagerSaveGame::StaticClass()));

	tmp->ProgressDay = ProgressDay;
	tmp->ExpirationDay = ExpirationDay;
	tmp->EventOccurLog = EventOccurLog;

	SaveManagerRef->Save(tmp, SaveActorType::EventManager);
}

void ATinyMetroEventManager::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UTinyMetroEventManagerSaveGame* tmp = Cast<UTinyMetroEventManagerSaveGame>(SaveManagerRef->Load(SaveActorType::EventManager));

	if (!IsValid(tmp)) {
		IsLoaded = false;
		return;
	}

	ProgressDay = tmp->ProgressDay;
	ExpirationDay = tmp->ExpirationDay;
	EventOccurLog = tmp->EventOccurLog;

	IsLoaded = true;
}

void ATinyMetroEventManager::SimulateProbability() {
	float EventProbability = (ProgressDay > (uint32)EventOccurProbability.Num()) ? 1.0 : EventOccurProbability[ProgressDay];
	//float EventProbability = 1.0f;
	if (EventProbability > FMath::RandRange(0.0, 1.0)) {
		UpdateEventWeight();
		OccurEvent();
		ProgressDay = 0;
	}
}

void ATinyMetroEventManager::OccurEvent() {
	UE_LOG(LogTemp, Log, TEXT("EventManager::OccurEvent"));
	int32 candidateIndex = FMath::RandRange(0, EventCandidate.Num() - 1);
	int32 occurEventId = EventCandidate[candidateIndex];

	auto SelectedEvent = (*LuaComponentArr.Find(occurEventId));
	if (SelectedEvent != nullptr) {
		//UE_LOG(LogTemp, Log, TEXT("EventManager::OccurEvent %d"), SelectedEvent->GetEventData().Id);
		FTinyMetroEventOccurData eventLog;
		eventLog.EventData = GetEventData(occurEventId);
		eventLog.Timestamp = TimerRef->GetTimestamp();
		ExpirationDay = eventLog.EventData.Period;
		EventOccurLog.Add(eventLog);

		SelectedEvent->LuaCallFunction(TEXT("Start"), TArray<FLuaValue>(), false);
		EventOccurTask.Broadcast();
		SoundManagerRef->PlaySound(TinyMetroEffectSound::OccurEvent);
	}
}

void ATinyMetroEventManager::UpdateEventWeight() {
	// Clear event wiehgt list
	EventCandidate.Empty();

	// Fill up event IDs by weight
	for (auto& i : LuaComponentArr) {
		int eventWeight = i.Value->LuaCallFunction(TEXT("Weight"), TArray<FLuaValue>(), false).ToInteger();
		for (int j = 0; j < eventWeight; j++) EventCandidate.Add(i.Key);
	}
}

FTinyMetroEventData ATinyMetroEventManager::GetEventData(int32 EventId) {
	FTinyMetroEventData eventData;
	if (LuaComponentArr.Find(EventId)) {
		FLuaValue luaEventData = LuaComponentArr[EventId]->LuaCallFunction(TEXT("EventData"), TArray<FLuaValue>(), false);
		eventData.Id = EventId;
		eventData.Title = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(luaEventData.GetField(TEXT("title")));
		eventData.FlavorText = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(luaEventData.GetField(TEXT("flavor_text")));
		eventData.Message = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(luaEventData.GetField(TEXT("message")));
		eventData.Period = luaEventData.GetField(TEXT("period")).ToInteger();
	} else {
		UE_LOG(LogTemp, Warning, TEXT("EventManager::GetEventData : Invalid Event Id"));
		return FTinyMetroEventData();
	}

	return eventData;
}

void ATinyMetroEventManager::InitializeEvent() {
	TArray<FString> findFiles;
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	FRegexPattern pattern(TEXT("Event\\.[1-9]\\d{3}\\.lua"));
	FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FString scriptDir = FPaths::ProjectContentDir()
		.Append(TEXT("Script"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Event"));

	if (!IsLoaded) {
		// Load fail : New game
		// Copy files to Content/Script/Event/Saved
		platformFile.FindFiles(findFiles, *scriptDir, TEXT("lua"));
		for (auto& i : findFiles) {
			FRegexMatcher matcher(pattern, i);
			if (matcher.FindNext()) {
				FString saveEventDir = FPaths::GetPath(i)
					.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
					.Append(TEXT("Saved"))
					.Append(mapName)
					.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
				FString cleanFileName = FPaths::GetCleanFilename(i);

				platformFile.DeleteDirectory(*saveEventDir);
				platformFile.CreateDirectory(*saveEventDir);
				//if (!platformFile.DirectoryExists(*saveEventDir)) platformFile.CreateDirectory(*saveEventDir);

				platformFile.CopyFile(*(saveEventDir + cleanFileName), *i);
			}
		}
	}

	scriptDir
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Saved"))
		.Append(mapName);

	findFiles.Empty();
	platformFile.FindFiles(findFiles, *scriptDir, TEXT("lua"));

	FString ScriptDirectory = TEXT("Script");
	ScriptDirectory.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Event"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Saved"))
		.Append(mapName)
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator());

	// Read files
	// Read directory = saved directory
	for (auto& i : findFiles) {
		FRegexMatcher matcher(pattern, i);
		if (matcher.FindNext()) {
			TArray<FString> parseArr;
			FPaths::GetCleanFilename(i).ParseIntoArray(parseArr, TEXT("."));
			int32 eventId =  FCString::Atoi(*parseArr[1]);
			/*auto tmpEvent = UTinyMetroEvent::CreateEvent(FPaths::GetCleanFilename(i), LuaState, GetWorld());
			EventArr.Emplace(tmpEvent->GetEventData().Id, tmpEvent);*/

			//FName name = *FString::Printf(TEXT("LuaComp %d"), tmpEvent->GetEventData().Id);
			auto tmpComponent = NewObject<ULuaComponent>(this, *FString::Printf(TEXT("LuaComp %d"), eventId));
			//tmpComp->RegisterComponent();
			tmpComponent->LuaState = UEventLuaState::StaticClass();
			//UE_LOG(LogTemp, Log, TEXT("Read... %s"), *ScriptDirectory);

			FLuaValue readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(GetWorld(), LuaState->GetClass(),
				ScriptDirectory + FPaths::GetCleanFilename(i), false);
			//UE_LOG(LogTemp, Log, TEXT("Read lua file... %d : %s"), tmpEvent->GetEventData().Id, *(ScriptDirectory + FPaths::GetCleanFilename(i)));
			
			tmpComponent->LuaSetField(TEXT("EventData"), readLua.GetField(TEXT("EventData")));
			tmpComponent->LuaSetField(TEXT("Weight"), readLua.GetField(TEXT("Weight")));
			tmpComponent->LuaSetField(TEXT("Start"), readLua.GetField(TEXT("Start")));

			/*TArray<FString> parseArr;
			FPaths::GetCleanFilename(i).ParseIntoArray(parseArr, TEXT("."));
			FLuaValue LuaEventdata = tmpComponent->LuaCallFunction(TEXT("EventData"), TArray<FLuaValue>(), false);
			FTinyMetroEventData EventData;
			EventData.Id = FCString::Atoi(*parseArr[1]);
			EventData.Title = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(LuaEventdata.GetField(TEXT("title")));
			EventData.FlavorText = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(LuaEventdata.GetField(TEXT("flavor_text")));
			EventData.Message = ULuaBlueprintFunctionLibrary::LuaValueToUTF8(LuaEventdata.GetField(TEXT("message")));
			EventData.Period = LuaEventdata.GetField(TEXT("period")).ToInteger();

			EventDataArr.Add(EventData.Id, EventData);*/
			LuaComponentArr.Add(eventId, tmpComponent);
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("EventManager::EventArr : %d"), EventArr.Num());
}

// Called every frame
void ATinyMetroEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

