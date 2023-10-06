// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/InvestmentManager.h"
#include "Finance/Investment.h"
#include "Finance/InvestmentManagerSaveGame.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManager.h"
#include "Station/StationManager.h"
#include "Train/TrainManager.h"
#include "Timer/Timer.h"
#include "Lua/InvestmentLuaState.h"
#include "LuaMachine/Public/LuaComponent.h"
#include "LuaMachine/Public/LuaBlueprintFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>

// Called when the game starts
void AInvestmentManager::BeginPlay() {
	Super::BeginPlay();

	// ...

	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(LuaState)) LuaState = UInvestmentLuaState::CreateInstance(GetWorld());
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();
	if (!IsValid(StationManagerRef)) StationManagerRef = GameModeRef->GetStationManager();
	if (!IsValid(TrainManagerRef)) TrainManagerRef = GameModeRef->GetTrainManager();

	InitInvestment();

	Load();

	RefreshTask.Broadcast();

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
			int32 investmentId = FCString::Atoi(*parseArr[1]);

			auto tmpComponent = Cast<UInvestment>(AddComponentByClass(UInvestment::StaticClass(), true, FTransform(), true));
			tmpComponent->LuaState = UInvestmentLuaState::StaticClass();

			FLuaValue readLua = ULuaBlueprintFunctionLibrary::LuaRunFile(GetWorld(), LuaState->GetClass(),
				ScriptDirectory + FPaths::GetCleanFilename(i), false);
			//UE_LOG(LogTemp, Log, TEXT("InvestmentManager::CreateInvestment"));
			
			tmpComponent->LuaSetField(TEXT("Id"), ULuaBlueprintFunctionLibrary::LuaCreateInteger(investmentId));
			tmpComponent->LuaSetField(TEXT("InvestmentData"), readLua.GetField(TEXT("InvestmentData")));
			tmpComponent->LuaSetField(TEXT("Start"), readLua.GetField(TEXT("Start")));
			tmpComponent->LuaSetField(TEXT("Appearance"), readLua.GetField(TEXT("Appearance")));
			tmpComponent->LuaSetField(TEXT("Process"), readLua.GetField(TEXT("Process")));
			tmpComponent->LuaSetField(TEXT("Award"), readLua.GetField(TEXT("Award")));
			//UE_LOG(LogTemp, Log, TEXT("InvestmentManager::InvestmentSetField"));

			FinishAddComponent(tmpComponent, true, FTransform());

			LuaComponentArr.Add(investmentId, tmpComponent);
			InvestmentIdArr.Add(investmentId);
		}
	}

	UpdateAccessibleInvestment();

	int32 randIdx = FMath::RandRange(0, InvestmentIdArr.Num() - 1);
	InvestmentCandidate.Add(0, InvestmentIdArr[randIdx]);

	if (LuaComponentArr.Num()< MaxInvestmentSlot) MaxInvestmentSlot = LuaComponentArr.Num();
}

void AInvestmentManager::NotifyInvestmentStart(int32 Id) {
	FInvestmentStartData tmp;
	tmp.Timestamp = TimerRef->GetTimestamp();
	tmp.DefaultStatistics = StatisticsManagerRef->DefaultStatistics;
	tmp.LaneStatistics = StatisticsManagerRef->LaneStatistics;
	tmp.ShopStatistics = StatisticsManagerRef->ShopStatistics;
	tmp.BankStatistics = StatisticsManagerRef->BankStatistics;
	tmp.StationInfoArr = StationManagerRef->GetAllStationInfo();
	for (auto& i : TrainManagerRef->GetAllTrain()) {
		tmp.TrainInfoArr.Add(i->GetTrainInfo());
	}

	InvestmentStartData.Add(Id, tmp);
}

void AInvestmentManager::NotifyInvestmentFinish(int32 Id) {
	InvestmentStartData.Remove(Id);
}

UInvestment* AInvestmentManager::GetInvestmentById(int32 Id) {
	return LuaComponentArr[Id];
}

TMap<int32, class UInvestment*> AInvestmentManager::GetAllInvestment() {
	return LuaComponentArr;
}

TMap<int32, int32> AInvestmentManager::GetInvestmentCandidate() {
	return InvestmentCandidate;
}

void AInvestmentManager::RefreshAccessibleInvestment() {
	UpdateAccessibleInvestment();
	TMap<int32, int32> newCandidate;
	for (int i = 0; i < InvestmentSlot; i++) {
		if (LuaComponentArr[InvestmentCandidate[i]]->GetState() == InvestmentState::Processing
			|| LuaComponentArr.Num() <= MaxInvestmentSlot * 2) {
			// If Investment is processing, not change investment
			newCandidate.Add(i, InvestmentCandidate[i]);
		} else {
			// Investment is finish or not started, change investment
			int32 randIdx = FMath::RandRange(0, InvestmentIdArr.Num() - 1);
			// Check : Not candidated investment
			while (InvestmentCandidate.FindKey(InvestmentIdArr[randIdx])
				|| newCandidate.FindKey(InvestmentIdArr[randIdx])) {
				randIdx = FMath::RandRange(0, InvestmentIdArr.Num() - 1);
			}
			newCandidate.Add(i, InvestmentIdArr[randIdx]);
		}
	}
	CanRefresh = false;
	InvestmentCandidate = newCandidate;
}

void AInvestmentManager::UpdateAccessibleInvestment() {
	InvestmentIdArr.Empty();
	for (auto& i : LuaComponentArr) {
		if (i.Value->GetAppearance()) {
			InvestmentIdArr.Add(i.Key);
		}
	}
}

void AInvestmentManager::WeeklyTask() {
	if (InvestmentSlot < MaxInvestmentSlot) {
		InvestmentCandidate.Add(InvestmentSlot++, *InvestmentIdArr.begin());
	}
	for (auto& i : LuaComponentArr) {
		if (i.Value->GetState() == InvestmentState::Fail || i.Value->GetState() == InvestmentState::Success) {
			i.Value->InitInvestment();
		}
	}
	RefreshAccessibleInvestment();
	CanRefresh = true;
	RefreshTask.Broadcast();
}

void AInvestmentManager::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UInvestmentManagerSaveGame* tmp = Cast<UInvestmentManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UInvestmentManagerSaveGame::StaticClass()));

	tmp->InvestmentStartData = InvestmentStartData;
	tmp->CanRefresh = CanRefresh;
	tmp->MaxInvestmentSlot = MaxInvestmentSlot;
	tmp->InvestmentSlot = InvestmentSlot;
	tmp->InvestmentCandidate = InvestmentCandidate;

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

	InvestmentStartData = tmp->InvestmentStartData;
	CanRefresh = tmp->CanRefresh;
	MaxInvestmentSlot = tmp->MaxInvestmentSlot;
	InvestmentSlot = tmp->InvestmentSlot;
	InvestmentCandidate = tmp->InvestmentCandidate;

	IsLoaded = true;
}

FInvestmentStartData AInvestmentManager::GetInvestmentStartData(int32 Id) {
	return InvestmentStartData.Contains(Id)? InvestmentStartData[Id] : FInvestmentStartData();
}
