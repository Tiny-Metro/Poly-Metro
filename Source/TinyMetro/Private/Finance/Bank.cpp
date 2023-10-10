// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Bank.h"
#include "Finance/BankSaveGame.h"
#include "Sample/SampleTimer.h"
#include "Station/StationManager.h"
#include "Timer/Timer.h"
#include "SaveSystem/TMSaveManager.h"
#include "Statistics/StatisticsManager.h"
#include "Lua/InvestmentLuaState.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
ABank::ABank()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABank::BeginPlay() {
	Super::BeginPlay();

	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	if (!IsValid(PlayerState)) PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();

	// Set daytime
	Daytime = GameModeRef->GetDaytime();

	IsLoadSuccess = Load();

	// Init Bank contents
	InitLoan();

	SaveManagerRef->SaveTask.AddDynamic(this, &ABank::Save);

	TimerRef->DailyTask.AddDynamic(this, &ABank::DailyTask);
	TimerRef->WeeklyTask.AddDynamic(this, &ABank::WeeklyTask);
}

TArray<ULoan*> ABank::GetAllLoan() const {
	return Loan;
}

ULoan* ABank::CreateLoan(FLoanData Data, TFunction<bool(void)> Func) {
	ULoan* Temp = NewObject<ULoan>();
	Temp->SetLoanData(Data);
	Temp->SetPlayerState(PlayerState);
	Temp->SetWorld(GetWorld());
	Temp->SetAvailabilityFunction(Func);
	Temp->SetStatisticsManager(StatisticsManagerRef);
	Temp->Load();

	return Temp;
}

void ABank::InitLoan() {
	// FLoanData(Amount, AutoRepay, Rate, Message)
	// Amount : Amount of loan
	// AutoRepay : Repay money per month
	// Rate : Rate of interest (5% = 1.05)
	// Message : Message for UI
	//TArray<FLoanData> LoanArr;
	TArray<TPair<FLoanData, TFunction<bool(void)>>> LoanArr;
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(0, 500, 0.03, 20, TEXT("")), [StatisticsManagerRef = StatisticsManagerRef]()->bool {
		return true;
		}));
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(1, 2000, 0.06, 20, TEXT("Sales over 3,000")), [StatisticsManagerRef = StatisticsManagerRef]()->bool {
		if (StatisticsManagerRef->DefaultStatistics.TotalIncome >= 3000) {
			return true;
		}
		return false;
		}));
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(2, 5000, 0.10, 20, TEXT("Profit over 10,000")), [StatisticsManagerRef = StatisticsManagerRef]()->bool {
		if (StatisticsManagerRef->DefaultStatistics.TotalIncome >= 10000) {
			return true;
		}
		return false;
		}));

	for (auto& i : LoanArr) {
		Loan.Add(this->CreateLoan(i.Key, i.Value));
	}
}

void ABank::DailyTask() {
	//for (auto& i : Investment) {
	//	i.Value->NotifyDailyTask();
	//}
}

void ABank::WeeklyTask() {
	// Loan weekly task
	for (auto& i : Loan) {
		i->NotifyWeeklyTask();
	}
}

void ABank::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UBankSaveGame* tmp = Cast<UBankSaveGame>(UGameplayStatics::CreateSaveGameObject(UBankSaveGame::StaticClass()));

	SaveManagerRef->Save(tmp, SaveActorType::Bank);

	for (auto& i : Loan) {
		i->Save();
	}
}

bool ABank::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UBankSaveGame* tmp = Cast<UBankSaveGame>(SaveManagerRef->Load(SaveActorType::Bank));

	if (!IsValid(tmp)) {
		return false;
	}

	return true;
}

void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (auto& i : Loan) {
		i->GetAvailable();
	}

}

