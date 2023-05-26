// Fill out your copyright notice in the Description page of Project Settings.


#include "Finance/Bank.h"
#include "Sample/SampleTimer.h"
#include "Station/StationManager.h"
#include "Finance/InvestmentLuaState.h"
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
	
	// Set GameMode
	GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());

	// Set PlayerState
	PlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	// Set daytime
	Daytime = GameModeRef->GetDaytime();

	// Set lua state
	LuaState = UInvestmentLuaState::CreateInstance(GetWorld());

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

	TArray<FString> findFiles;
	FString scriptDir = FPaths::ProjectContentDir()
		.Append(TEXT("Script"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(TEXT("Investment"));
	FRegexPattern pattern(TEXT("Investment\\.[1-9]\\d{3}\\.lua"));
	FPlatformFileManager::Get().GetPlatformFile().FindFiles(findFiles, *scriptDir, TEXT("lua"));
	for (auto& i : findFiles) {
		FRegexMatcher matcher(pattern, i);
		if (matcher.FindNext()) {
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan,
				FString::Printf(TEXT("Files : %s"), *FPaths::GetCleanFilename(i)));
			Investment.Add(UInvestment::CreateInvestment(FPaths::GetCleanFilename(i), LuaState, GetWorld()));
		}
	}
	
	/*Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("환승 시스템 요청"),
			TEXT("대중교통 간 환승 시스템을 도입해야 한다는 목소리가 커지고 있습니다. 정부에서는 환승 시스템을 20일간 시범 도입하는 방안을 고려하고 있습니다."),
			20,
			TEXT("환승 시스템 20일 동안 활성화"),
			TEXT("+300$"),
			TEXT("+300$, 7일간 승객 20% 증가"),
			TEXT("-300$, 7일간 승객 20% 감소")),
		Daytime,
		[PlayerState = PlayerState]() {
			PlayerState->AddMoney(300);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 0"));
		},
		[PlayerState = PlayerState, StationManagerRef = GameModeRef->GetStationManager()]() {
			PlayerState->AddMoney(300);
			StationManagerRef->AddPassengerSpawnProbability(0.2f, 7);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 0"));
		},
		[PlayerState = PlayerState, StationManagerRef = GameModeRef->GetStationManager()]() {
			PlayerState->AddMoney(-600);
			StationManagerRef->AddPassengerSpawnProbability(-0.2f, 7);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 0"));
		},
		[PolicyRef = GameModeRef->GetPolicy()]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 0"));
			if (PolicyRef->GetTransfer()) {
				return InvestmentState::Success;
			} else {
				return InvestmentState::Fail;
			}
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("역 후보지 활성화"),
			TEXT("“우리 집 앞에도 역을 만들어 달라!” 역 추가 건설에 대한 요구가 쇄도하고 있습니다. 새 역 건설에 여러 회사들이 투자하겠다고 제안했습니다."),
			10,
			TEXT("10일 이내 역 후보지 활성화"),
			TEXT("+300$"),
			TEXT("+500$, 역 후보지의 불만도 5% 감소"),
			TEXT("-100$, 역 후보지의 불만도 5% 증가")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 1"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 1"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 1"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 1"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("엘리베이터 설치"),
			TEXT("노약자와 장애인 등 교통약자들이 역을 이용하는데 어려움을 겪고 있습니다. 정부에서는 엘리베이터 설치에 대해 지원을 약속했습니다."),
			20,
			TEXT("20일 이내 엘리베이터 설치"),
			TEXT("+1,000$, 승객 50% 증가"),
			TEXT("+3,000$"),
			TEXT("-2,000$")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 2"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 2"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 2"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 2"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("CCTV 설치"),
			TEXT("지하철 범죄가 기승을 부리고 있습니다. 범죄율을 낮추기 위해 CCTV를 설치해야 한다는 의견이 제기되었습니다."),
			20,
			TEXT("20일 이내 CCTV 설치"),
			TEXT("+1,000$, 승객 50% 증가"),
			TEXT("+3,000$"),
			TEXT("-2,000$")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 3"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 3"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 3"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 3"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("노약자석/임산부석 설치"),
			TEXT("교통약자를 위해 노약자석과 임산부석을 설치해야 한다는 의견이 대두되고 있습니다."),
			20,
			TEXT("20일 이내 노약자석/임산부석 설치"),
			TEXT("+1,000$, 승객 50% 증가"),
			TEXT("+3,000$"),
			TEXT("-2,000$")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 4"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 4"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 4"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 4"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("노선 추가"),
			TEXT("도시가 성장해나감에 따라 새로운 노선 개통의 필요성이 제기되었습니다. 새 노선을 건설하는데 투자하기 위한 컨소시엄이 구성되었습니다."),
			20,
			TEXT("20일 이내 신규 노선 생성"),
			TEXT("+300$"),
			TEXT("+1,500$"),
			TEXT("-500$")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 5"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 5"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 5"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 5"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("동차 추가"),
			TEXT("호선에 승객이 몰려 배차 간격에 대한 불만이 급증하고 있습니다. 동차를 추가적으로 배치하는 방안에 대해 고려해야 할 시점입니다."),
			10,
			TEXT("10일 이내 해당 노선에 동차 1개 추가"),
			TEXT("+50$"),
			TEXT("+150$"),
			TEXT("-70$")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 6"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 6"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 6"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 6"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("승객 수송"),
			TEXT("대중교통 환경 평가 기간입니다. 승객을 최대한 많이 목적지로 모셔 좋은 평가를 받도록 노력해야 합니다."),
			10,
			TEXT("10일 이내 승객 N명 목표 지역으로 수송"),
			TEXT("승객 40% 증가"),
			TEXT("+100$, 해당 역 불만도 5% 감소"),
			TEXT("-40$, 역 불만도 5% 증가")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 7"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 7"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 7"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 7"));
			return InvestmentState::Processing;
		}
	));
	Investment.Add(UInvestment::CreateInvestment(
		FInvestmentData(
			TEXT("동차/객차 업그레이드"),
			TEXT("승객들이 좁은 열차 안에서 신음하고 있습니다. 열차의 수송 인원을 늘리기 위해 새 열차 도입이 고려되고 있습니다."),
			20,
			TEXT("10일 이내 동차 혹은 객차 3개 업그레이드"),
			TEXT("+100$"),
			TEXT("+500$"),
			TEXT("-300$")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 8"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 8"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 8"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 8"));
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
			TEXT("+300"),
			TEXT("-300")),
		Daytime,
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Start 9"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Success 9"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Fail 9"));
		},
		[]() {
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Check 9"));
			return InvestmentState::Processing;
		}
	));*/

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

