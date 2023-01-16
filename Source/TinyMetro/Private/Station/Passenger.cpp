// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Passenger.h"

void UPassenger::SetDestination(StationType Dest) {
	Destination = Dest;
}

UPassenger* UPassenger::ConstructPassenger(StationType Dest) {
	auto Obj = NewObject<UPassenger>();
	Obj->SetDestination(Dest);
	Obj->SetFree();
	return Obj;
}

void UPassenger::SetFree() {
	int result = FMath::RandRange(0,4);

	IsFree = RandomFree[result];
}