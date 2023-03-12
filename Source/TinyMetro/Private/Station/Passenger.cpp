// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Passenger.h"

void UPassenger::SetDestination(StationType Dest) {
	Destination = Dest;
}

StationType UPassenger::GetDestination() const {
	return Destination;
}

UPassenger* UPassenger::ConstructPassenger(StationType Dest) {
	auto Obj = NewObject<UPassenger>();
	Obj->SetDestination(Dest);
	//Obj->SetFree();  TODO : if Policy->GetHasBicycle == true
	return MoveTemp(Obj);
}

void UPassenger::SetFree() {
	int result = FMath::RandRange(0,4);

	IsFree = RandomFree[result];
}

void UPassenger::SetPassengerRoute(TQueue<int32>* Route) {
	// Remove start station
	if (Route != nullptr) {
		Route->Pop();
	}
	ShortestRoute = Route;
}

TQueue<int32>* UPassenger::GetPassengerRoute() {
	return ShortestRoute;
}
