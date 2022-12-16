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

// Called when the game starts or when spawned
void ABank::BeginPlay()
{
	Super::BeginPlay();

	// Set PlayerState
	PlayerState = GetWorld()->GetControllerIterator()->Get()->GetPlayerState<ATinyMetroPlayerState>();

	// Set loan data
	InitLoan();
}

ULoan* ABank::CreateLoan(FLoanData Data, TFunction<bool(void)> Func) {
	ULoan* Temp = NewObject<ULoan>();
	Temp->SetLoanData(Data);
	//ATinyMetroGameModeBase* GameMode = (ATinyMetroGameModeBase*)GetWorld()->GetAuthGameMode();
	//Daytime = GameMode->GetDaytime();
	Temp->SetPlayerState(PlayerState);
	Temp->SetDaytime(Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetDaytime());
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
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(500, 20, 1.03, TEXT("")), [PlayerState = PlayerState]()->bool {
		return true;
		}));
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(2000, 80, 1.06, TEXT("Sales over 3,000")), [PlayerState = PlayerState]()->bool {
		if (PlayerState->GetSales() >= 3000) {
			return true;
		}
		return false;
		}));
	LoanArr.Add(TPair<FLoanData, TFunction<bool(void)>>(FLoanData(5000, 200, 1.10, TEXT("Profit over 10,000")), [PlayerState = PlayerState]()->bool {
		if (PlayerState->GetProfit() >= 10000) {
			return true;
		}
		return false;
		}));

	for (auto& i : LoanArr) {
		Loan.Add(this->CreateLoan(i.Key, i.Value));
	}
}

// Called every frame
void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

