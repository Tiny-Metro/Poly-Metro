// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/InvestmentManager.h"
#include "Finance/Investment.h"
#include "Finance/InvestmentManagerSaveGame.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "SaveSystem/TMSaveManager.h"
#include "Timer/Timer.h"
#include "Lua/InvestmentLuaState.h"
#include "LuaMachine/Public/LuaComponent.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include <Kismet/GameplayStatics.h>

// Called when the game starts
void AInvestmentManager::BeginPlay() {
	Super::BeginPlay();

	// ...

	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(LuaState)) LuaState = UInvestmentLuaState::CreateInstance(GetWorld());

	InitInvestment();

	TimerRef->DailyTask.AddDynamic(this, &AInvestmentManager::DailyTask);
	TimerRef->WeeklyTask.AddDynamic(this, &AInvestmentManager::WeeklyTask);

	SaveManagerRef->SaveTask.AddDynamic(this, &AInvestmentManager::Save);
}

// Sets default values for this component's properties
AInvestmentManager::AInvestmentManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	// ...
}

void AInvestmentManager::InitInvestment() {
	TArray<FString> findFiles;
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	FRegexPattern pattern(TEXT("Investment\\.[1-9]\\d{3}\\.lua"));
	FString mapName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	FString scriptDir = FPaths::ProjectContentDir()
		.Append(TEXT("Script"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Investment"));

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
		.Append(TEXT("Investment"))
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
			int32 eventId = FCString::Atoi(*parseArr[1]);

			auto tmpComponent = Cast<UInvestment>(AddComponentByClass(UInvestment::StaticClass(), true, FTransform(), true));
			tmpComponent->LuaState = UInvestmentLuaState::StaticClass();

			FLuaValue readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(GetWorld(), LuaState->GetClass(),
				ScriptDirectory + FPaths::GetCleanFilename(i), false);
			UE_LOG(LogTemp, Log, TEXT("InvestmentManager::CreateInvestment"));
			
			tmpComponent->LuaSetField(TEXT("Id"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(eventId));
			tmpComponent->LuaSetField(TEXT("InvestmentData"), readLua.GetField(TEXT("InvestmentData")));
			tmpComponent->LuaSetField(TEXT("Start"), readLua.GetField(TEXT("Start")));
			tmpComponent->LuaSetField(TEXT("Appearance"), readLua.GetField(TEXT("Appearance")));
			tmpComponent->LuaSetField(TEXT("Award"), readLua.GetField(TEXT("Award")));
			UE_LOG(LogTemp, Log, TEXT("InvestmentManager::InvestmentSetField"));

			FinishAddComponent(tmpComponent, true, FTransform());

			LuaComponentArr.Add(eventId, tmpComponent);
		}
	}
}

void AInvestmentManager::NotifyInvestmentStart(int32 Id) {
}

void AInvestmentManager::NotifyInvestmentFinish(int32 Id) {
}

UInvestment* AInvestmentManager::GetInvestmentById(int32 Id) {
	return LuaComponentArr[Id];
}

void AInvestmentManager::DailyTask() {
}

void AInvestmentManager::WeeklyTask() {
	if (InvestmentSlot < MaxInvestmentSlot) InvestmentSlot++;
	CanRefresh = true;
}

void AInvestmentManager::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UInvestmentManagerSaveGame* tmp = Cast<UInvestmentManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UInvestmentManagerSaveGame::StaticClass()));

	// TODO : Store save data
	//tmp->IsStart = IsStart;

	SaveManagerRef->Save(tmp, SaveActorType::InvestmentManager);
}

void AInvestmentManager::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UInvestmentManagerSaveGame* tmp = Cast<UInvestmentManagerSaveGame>(SaveManagerRef->Load(SaveActorType::InvestmentManager));

	if (!IsValid(tmp)) {
		return;
	}

	// TODO : Read save data
	//IsStart = tmp->IsStart;
}
