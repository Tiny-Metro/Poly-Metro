// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Bank.h"
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
	if (!IsValid(LuaState)) LuaState = UInvestmentLuaState::CreateInstance(GetWorld());
	if (!IsValid(TimerRef)) TimerRef = GameModeRef->GetTimer();
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();

	// Set daytime
	Daytime = GameModeRef->GetDaytime();

	IsLoadSuccess = Load();

	// Init Bank contents
	InitLoan();
	InitInvestment();

	ChangeAllInvestment();

	TimerRef->DailyTask.AddDynamic(this, &ABank::DailyTask);
	TimerRef->WeeklyTask.AddDynamic(this, &ABank::WeeklyTask);
}

TArray<ULoan*> ABank::GetAllLoan() const {
	return Loan;
}

void ABank::InitInvestment() {
	//TArray<FString> findFiles;
	//IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	//FString scriptDir = FPaths::ProjectContentDir()
	//	.Append(TEXT("Script"))
	//	.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
	//	.Append(TEXT("Investment"));
	//FRegexPattern pattern(TEXT("Investment\\.[1-9]\\d{3}\\.lua"));


	//if (!IsLoadSuccess) {
	//	// Load fail : New game
	//	// Copy files to Content/Script/Investment/Saved
	//	platformFile.FindFiles(findFiles, *scriptDir, TEXT("lua"));
	//	for (auto& i : findFiles) {
	//		FRegexMatcher matcher(pattern, i);
	//		if (matcher.FindNext()) {
	//			FString saveEventDir = FPaths::GetPath(i)
	//				.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
	//				.Append(TEXT("Saved"))
	//				.Append(FGenericPlatformMisc::GetDefaultPathSeparator());
	//			FString cleanFileName = FPaths::GetCleanFilename(i);

	//			if (!platformFile.DirectoryExists(*saveEventDir)) platformFile.CreateDirectory(*saveEventDir);

	//			platformFile.CopyFile(*(saveEventDir + cleanFileName), *i);
	//		}
	//	}
	//}

	//scriptDir
	//	.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
	//	.Append(TEXT("Saved"));

	//findFiles.Empty();
	//platformFile.FindFiles(findFiles, *scriptDir, TEXT("lua"));

	//// Read files
	//// Read directory = saved directory
	//for (auto& i : findFiles) {
	//	FRegexMatcher matcher(pattern, i);
	//	if (matcher.FindNext()) {
	//		auto tmpInvestment = UInvestment::CreateInvestment(FPaths::GetCleanFilename(i), LuaState, GetWorld());
	//		Investment.Add(tmpInvestment->GetInvestmentData().Id, tmpInvestment);
	//		InvestmentCandidate.Add(tmpInvestment->GetInvestmentData().Id);
	//	}
	//}
}

// Replace investment at Index
// If Index = -1, add investment at last
// If investment are full, remove old investment
void ABank::ChangeInvestment() {
	UpdateInvestmentCandidate();
	RemoveFinishedInvestment();

	//// Remove duplicated index
	//for (auto& i : VisibleInvestmentIndex) {
	//	InvestmentCandidate.RemoveSingle(i);
	//}

	//// If stock is full, remove not processing one
	//if (VisibleInvestmentIndex.Num() == InvestmentStock) {
	//	bool removeFlag = false;
	//	for (auto& i : VisibleInvestmentIndex) {
	//		if (!removeFlag) {
	//			auto state = Investment[i]->GetInvestmentState();
	//			if (state != InvestmentState::Processing) {
	//				VisibleInvestmentIndex.RemoveSingle(i);
	//				removeFlag = true;
	//			}
	//		}
	//	}
	//}

	//// Check number of investment
	//// Fill up investment
	//while (InvestmentCandidate.Num() > 0 && VisibleInvestmentIndex.Num() < InvestmentStock) {
	//	int32 newInvestmentId = InvestmentCandidate[FMath::RandRange(0, InvestmentCandidate.Num() - 1)];
	//	VisibleInvestmentIndex.Add(newInvestmentId);
	//	InvestmentCandidate.RemoveSingle(newInvestmentId);
	//}
}

// Replace all investment except activated one
void ABank::ChangeAllInvestment() {
	UpdateInvestmentCandidate();

	//// Remove duplicated index
	//for (auto& i : VisibleInvestmentIndex) {
	//	InvestmentCandidate.RemoveSingle(i);
	//}

	//// Clear investment slot except processing one
	//for (auto& i : VisibleInvestmentIndex) {
	//	auto state = Investment[i]->GetInvestmentState();
	//	if (state != InvestmentState::Processing) VisibleInvestmentIndex.RemoveSingle(i);
	//}

	//// Check number of investment
	//// Fill up investment
	//while (InvestmentCandidate.Num() > 0 && VisibleInvestmentIndex.Num() < InvestmentStock) {
	//	int32 newInvestmentId = InvestmentCandidate[FMath::RandRange(0, InvestmentCandidate.Num() - 1)];
	//	VisibleInvestmentIndex.Add(newInvestmentId);
	//	InvestmentCandidate.RemoveSingle(newInvestmentId);
	//}
}

int32 ABank::GetInvestmentStock() const {
	return int32();
}

void ABank::UpdateInvestmentCandidate() {
	/*InvestmentCandidate.Empty();
	for (auto& i : Investment) {
		if (i.Value->GetAppearance()) {
			InvestmentCandidate.Add(i.Key);
		}
	}*/
}

void ABank::RemoveFinishedInvestment() {
	// Remove success, fail investment
	/*for (auto& i : VisibleInvestmentIndex) {
		auto state = Investment[i]->GetInvestmentState();
		if (state == InvestmentState::Success || state == InvestmentState::Fail) {
			Investment[i]->ResetInvestment();
			VisibleInvestmentIndex.RemoveSingle(i);
		}
	}*/
}


ULoan* ABank::CreateLoan(FLoanData Data, TFunction<bool(void)> Func) {
	ULoan* Temp = NewObject<ULoan>();
	Temp->SetLoanData(Data);
	//ATinyMetroGameModeBase* GameMode = (ATinyMetroGameModeBase*)GetWorld()->GetAuthGameMode();
	//Daytime = GameMode->GetDaytime();
	Temp->SetPlayerState(PlayerState);
	Temp->SetDaytime(Daytime);
	Temp->SetWorld(GetWorld());
	Temp->SetAvailabilityFunction(Func);
	Temp->SetStatisticsManager(StatisticsManagerRef);
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
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(500, 0.03, 20, TEXT("")), [PlayerState = PlayerState]()->bool {
		return true;
		}));
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(2000, 0.06, 20, TEXT("Sales over 3,000")), [PlayerState = PlayerState]()->bool {
		if (PlayerState->GetSales() >= 3000) {
			return true;
		}
		return false;
		}));
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(5000, 0.10, 20, TEXT("Profit over 10,000")), [PlayerState = PlayerState]()->bool {
		if (PlayerState->GetProfit() >= 10000) {
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

	// Investment weekly task
	//if (InvestmentStock < MaxInvestmetStock) InvestmentStock++;
	//ChangeInvestment();
	//InvestmentUpdateTask.Broadcast();
}

void ABank::Save() {
}

bool ABank::Load() {
	return false;
}

void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//for (auto& i : Investment) {
	//	i.Value->InvestmentProcess();
	//}

	for (auto& i : Loan) {
		i->GetAvailable();
	}

}

