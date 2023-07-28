// Fill out your copyright notice in the Description page of Project Settings.


#include "Statistics/StatisticsManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Timer/Timer.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManagerSaveGame.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AStatisticsManager::AStatisticsManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

AStatisticsManager::~AStatisticsManager() {
	delete StatisticsKey;
}

// Called when the game starts or when spawned
void AStatisticsManager::BeginPlay()
{
	Super::BeginPlay();

	// Get outer actors references
	if (GameModeRef == nullptr) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TimerRef == nullptr) TimerRef = GameModeRef->GetTimer();
	if (SaveManagerRef == nullptr) SaveManagerRef = GameModeRef->GetSaveManager();
	

	// Bind daily, weekly tasks
	TimerRef->DailyTask.AddDynamic(this, &AStatisticsManager::DailyTask);
	TimerRef->WeeklyTask.AddDynamic(this, &AStatisticsManager::WeeklyTask);

	// Bind save task
	SaveManagerRef->SaveTask.AddDynamic(this, &AStatisticsManager::Save);
}

// Called every frame
void AStatisticsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStatisticsManager::DailyTask() {
}

void AStatisticsManager::WeeklyTask() {
	// Reset weekly values (Default statistics)
	DefaultStatistics.WeeklyArrivePassenger = 0;
	DefaultStatistics.WeeklyIncome = 0;
	DefaultStatistics.WeeklySpending = 0;

	// Reset weekly values (Lane statistics)
	for (auto& i : LaneStatistics.Lanes) {
		i.Value.WeeklyArrivePassenger = 0;
		i.Value.WeeklyProfit = 0;
	}

}

void AStatisticsManager::Save() {
	UStatisticsManagerSaveGame* tmp = Cast<UStatisticsManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UStatisticsManagerSaveGame::StaticClass()));
	tmp->DefaultStatistics = DefaultStatistics;
	tmp->LaneStatistics = LaneStatistics;
	tmp->ShopStatistics = ShopStatistics;
	tmp->BankStatistics = BankStatistics;

	SaveManagerRef->Save(tmp, SaveActorType::StatisticsManager);
}

void AStatisticsManager::Load() {
	// Get outer actors references
	if (GameModeRef == nullptr) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (SaveManagerRef == nullptr) SaveManagerRef = GameModeRef->GetSaveManager();

	UStatisticsManagerSaveGame* tmp = Cast<UStatisticsManagerSaveGame>(SaveManagerRef->Load(SaveActorType::StatisticsManager));

	if (!IsValid(tmp)) return;

	DefaultStatistics = tmp->DefaultStatistics;
	LaneStatistics = tmp->LaneStatistics;
	ShopStatistics = tmp->ShopStatistics;
	BankStatistics = tmp->BankStatistics;

}

FCriticalSection* AStatisticsManager::GetKey() const {
	return StatisticsKey;
}

