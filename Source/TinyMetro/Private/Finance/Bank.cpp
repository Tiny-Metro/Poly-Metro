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

	// Set loan data
	// FLoanData(Amount, AutoRepay, Rate, Message)
	// Amount : Amount of loan
	// AutoRepay : Repay money per month
	// Rate : Rate of interest (5% = 1.05)
	// Message : Message for UI
	TArray<FLoanData> LoanArr;
	LoanArr.Add(FLoanData(500, 20, 1.03, TEXT("")));
	LoanArr.Add(FLoanData(2000, 80, 1.06, TEXT("Sales over 3,000")));
	LoanArr.Add(FLoanData(5000, 200, 1.10, TEXT("Profit over 10,000")));

	for (auto& i : LoanArr) {
		Loan.Add(this->CreateLoan(i));
	}

	
	
}

ULoan* ABank::CreateLoan(FLoanData Data) {
	ULoan* Temp = NewObject<ULoan>();
	Temp->SetLoanData(Data);
	//Temp->SetDaytime();
	return Temp;
}

// Called every frame
void ABank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

