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

TArray<UInvestment*> ABank::GetAllInvestment() const {
	return Investment;
}

TArray<UInvestment*> ABank::GetAvailableInvestment() const {
	TArray<UInvestment*> temp;
	for (auto& i : AvailInvestment) {
		temp.Add(Investment[i]);
	}
	return temp;
}

TArray<UInvestment*> ABank::RefreshInvestment() {
	ChangeAllInvestment();
	return GetAvailableInvestment();
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
	AvailInvestment.Add(FMath::RandRange(0, Investment.Num() - 1));

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

void ABank::ChangeInvestment(int Index = -1) {
	int AvailIndex;
	bool flag = true;
	do {
		AvailIndex = FMath::RandRange(0, Investment.Num() - 1);
		if (AvailInvestment.Find(AvailIndex) == INDEX_NONE) {
			if (Index == -1) {
				AvailInvestment.Add(AvailIndex);
				AvailInvestment.RemoveAt(0);
			} else {
				AvailInvestment.Insert(AvailIndex, Index);
				if (InvestmentStock >= MaxInvestmetStock) {
					AvailInvestment.RemoveAt(Index + 1);
				}
			}
			flag = false;
		}
	} while (flag);
}

void ABank::ChangeAllInvestment() {
	TArray<int32> temp;
	int AvailIndex;
	int flag = 0;
	do {
		AvailIndex = FMath::RandRange(0, Investment.Num() - 1);
		if (Investment[AvailInvestment[flag]]->GetIsActivate()) {
			temp.Add(AvailInvestment[flag]);
			flag++;
		} else {
			if (AvailInvestment.Find(AvailIndex) == INDEX_NONE) {
				temp.Add(AvailIndex);
				flag++;
			}
		}
	} while (flag >= InvestmentStock);

	AvailInvestment.Empty();
	for (auto& i : temp) {
		AvailInvestment.Add(i);
	}

}

void ABank::UpdateInvestment() {
	GetWorld()->GetTimerManager().SetTimer(
		InvestmentUpdateHandle,
		FTimerDelegate::CreateLambda([&InvestmentStock = InvestmentStock, this]() {
			if (InvestmentStock < MaxInvestmetStock) {
				this->ChangeInvestment(InvestmentStock);
				InvestmentStock++;
			} else {
				this->ChangeInvestment();
			}
		}),
		Daytime * 7,
		true,
		(Daytime * 7) - (FMath::Fmod(GetWorld()->GetTimeSeconds(), Daytime * 7))
	);
}

// Called every frame
void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

