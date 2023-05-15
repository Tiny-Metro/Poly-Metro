// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Bank.h"
#include "Sample/SampleTimer.h"
#include <Kismet/GameplayStatics.h>

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
		temp.Emplace(Investment[i]);
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
	PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	// Set loan data
	InitLoan();

	// Set investment data
	InitInvestment();
	//UpdateInvestment();
	AvailInvestment.Emplace(FMath::RandRange(0, Investment.Num() - 1));

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
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("0환승 시스템 요청"),
			TEXT("0대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("1환승 시스템 요청"),
			TEXT("1대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("2환승 시스템 요청"),
			TEXT("2대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("3환승 시스템 요청"),
			TEXT("3대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("4환승 시스템 요청"),
			TEXT("4대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("5환승 시스템 요청"),
			TEXT("5대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("6환승 시스템 요청"),
			TEXT("6대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("7환승 시스템 요청"),
			TEXT("7대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("8환승 시스템 요청"),
			TEXT("8대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("9환승 시스템 요청"),
			TEXT("9대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("+300"),
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {

		},
		[]() {

		},
		[]() {

		},
		[]() {
			return InvestmentState::Processing;
		}
	));

	//InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(
	//	Tmp,
	//	[]() {

	//		return false;
	//		}
	//	)
	//);
	/*InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(10, 300, 500, 700, TEXT("Message : 2")), []() {

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
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 50, 150, 70, TEXT("Message : 6")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 50, 150, 70, TEXT("Message : 7")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 50, 150, 70, TEXT("Message : 8")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 50, 150, 70, TEXT("Message : 9")), []() {

		return false;
		}));
	InvestmentArr.Add(TPair<FInvestmentData, TFunction<bool(void)>>(FInvestmentData(20, 50, 150, 70, TEXT("Message : 10")), []() {

		return false;
		}));*/

	/*for (auto& i : InvestmentArr) {
		Investment.Add(this->CreateInvestment(i.Key, i.Value));
	}*/
}

//UInvestment* ABank::CreateInvestment(FInvestmentData Data, TFunction<InvestmentResult(void)> Func) {
//	UInvestment* temp = NewObject<UInvestment>();
//	temp->SetInvestmentData(Data);
//	temp->SetDaytime(Daytime);
//	temp->SetPlayerState(PlayerState);
//	temp->SetWorld(GetWorld());
//	temp->SetSuccessFunction(Func);
//	return temp;
//}

// Replace investment at Index
// If Index = -1, add investment at last
// If investment are full, remove old investment
void ABank::ChangeInvestment(int Index = -1) {
	int AvailIndex;
	bool flag = true;
	int removeIndex = -1;

	// Select remove index
	for (auto& i : AvailInvestment) {
		if (Investment[i]->GetState() != InvestmentState::Processing) {
			removeIndex = i;
			break;
		}
	}
	
	// If all investment are activated, change nothing
	if (removeIndex == -1) return;

	do {
		// Pick random number
		// If random number not included AvailInvestment
		// Random number insert the AvailInvestment
		AvailIndex = FMath::RandRange(0, Investment.Num() - 1);
		if (AvailInvestment.Find(AvailIndex) == INDEX_NONE) {
			if (Index == -1) {
				AvailInvestment.Add(AvailIndex);
				if (AvailInvestment.Num() > MaxInvestmetStock) {
					AvailInvestment.RemoveAt(AvailInvestment.Find(removeIndex));
				}
			} else {
				AvailInvestment.Insert(AvailIndex, Index);
				if (AvailInvestment.Num() > MaxInvestmetStock) {
					AvailInvestment.RemoveAt(Index + 1);
				}
			}
			flag = false;
		}
	} while (flag);
}

// Replace all investment except activated one
void ABank::ChangeAllInvestment() {
	TArray<int32> Temp;
	int AvailIndex;
	int Flag = 0;
	do {
		AvailIndex = FMath::RandRange(0, Investment.Num() - 1);
		int Idx = AvailInvestment[Flag];
		if (Investment[Idx]->GetState() == InvestmentState::Processing) {
			Temp.Emplace(Idx);
			Flag++;
		} else {
			if (AvailInvestment.Find(AvailIndex) == INDEX_NONE &&
				Temp.Find(AvailIndex) == INDEX_NONE) {
				Temp.Emplace(AvailIndex);
				Flag++;
			}
		}
	} while (Flag < InvestmentStock);
	
	//AvailInvestment = temp;
	AvailInvestment.Empty();
	for (auto i : Temp) {
		AvailInvestment.Emplace(i);
	}

}

void ABank::UpdateInvestment() {
	if (InvestmentStock < MaxInvestmetStock) {
		InvestmentStock++;
	} 
	this->ChangeInvestment();
	/*GetWorld()->GetTimerManager().SetTimer(
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
	);*/
}

void ABank::DailyTask() {
	for (auto& i : Investment) {
		i->NotifyDailyTask();
	}
}

void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

