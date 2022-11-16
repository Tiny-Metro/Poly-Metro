// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Station.h"

// Sets default values
AStation::AStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,
			FString::Printf(TEXT("I am %s"), *this->GetActorLabel()));
	}


}

// Called every frame
void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStation::SetStationId(int32 Id) {
	StationId = Id;
}

void AStation::SetStationType(StationType Type) {
	StationTypeValue = Type;

	//LOG
	FString EnumToStr = TEXT("NULL");
	const UEnum* MyType = FindObject<UEnum>(ANY_PACKAGE, TEXT("StationType"), true);
	if (MyType) {
		EnumToStr = MyType->GetNameStringByValue((int64)StationTypeValue);
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Cyan,
			EnumToStr);
	}
}

void AStation::CalculateComplain() {
}

void AStation::ActivateStation() {
	IsActive = true;

	// TODO :  Visible logic
}

