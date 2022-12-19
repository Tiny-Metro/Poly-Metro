// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Bank.h"
#include "Sample/SampleTimer.h"

// Sets default values
ABank::ABank()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

TArray<ULoan*> ABank::GetAllLoan() const {
	return Loan;
}

int32 ABank::GetInvestmentStock() const {
	return int32();
}

// Called when the game starts or when spawned
void ABank::BeginPlay()
{
	Super::BeginPlay();
	
	// Set daytime
	Daytime = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetDaytime();

	// Set PlayerState
	PlayerState = GetWorld()->GetControllerIterator()->Get()->GetPlayerState<ATinyMetroPlayerState>();

	// Set loan data
	InitLoan();

	// Set investment data
	InitInvestment();
	UpdateInvestment();

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

void ABank::InitInvestment() {
	TArray<TPair<FInvestmentData, TFunction<bool(void)>>> InvestmentArr;

	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 300, 300, 100, TEXT("Message : 1")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(10, 300, 500, 700, TEXT("Message : 2")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 1000, 3000, 2000, TEXT("Message : 3")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 300, 1500, 500, TEXT("Message : 4")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 50, 150, 70, TEXT("Message : 5")), []() {

		return false;
		}));

	for (auto& i : InvestmentArr) {
		Investment.Add(this->CreateInvestment(i.Key, i.Value));
	}
}

UInvestment* ABank::CreateInvestment(FInvestmentData Data, TFunction<bool(void)> Func) {
	UInvestment* temp = NewObject<UInvestment>();
	temp->SetInvestmentData(Data);
	temp->SetDaytime(Daytime);
	temp->SetPlayerState(PlayerState);
	temp->SetWorld(GetWorld());
	temp->SetSuccessFunction(Func);
	return temp;
}

void ABank::UpdateInvestment() {
}

// Called every frame
void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

