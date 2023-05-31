// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainManager.h"
#include "Train/Train.h"
#include "Train/Subtrain.h"
#include "Lane/Lane.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include <Engine/AssetManager.h>

// Sets default values
ATrainManager::ATrainManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATrainManager::BeginPlay()
{
	Super::BeginPlay();

	InitTrainMaterial();
	InitPassengerMaterial();
}

void ATrainManager::AddTrain(ATrainTemplate* Train) {
	RefreshTrainArray();
	if (Trains.Find(Train) == INDEX_NONE) {
		Train->SetTrainId(NextTrainId++);
		Trains.AddUnique(Train);
	}
}

void ATrainManager::RemoveTrain(ATrainTemplate* Train) {
	if (IsValid(Train)) {
		Trains.Remove(Train);
	}
}

void ATrainManager::SetTrainId(ATrainTemplate* Train) {
	Train->SetTrainId(NextTrainId++);
}

ATrainTemplate* ATrainManager::GetTrainById(int32 TrainId, TrainType& Type) {
	for (auto i : Trains) {
		if (TrainId == i->GetTrainId()) {
			if (i->IsA(ATrain::StaticClass())) Type = TrainType::Train;
			else Type = TrainType::SubTrain;
			return i;
		}
	}
	return nullptr;
}

ATrain* ATrainManager::GetNearestTrain(FVector CurrentLocation, class ALane* LaneRef = nullptr) {
	double Distance = std::numeric_limits<double>::max();
	int TrainIndex = 0;
	bool LaneValid = IsValid(LaneRef);
	for (int i = 0; i < Trains.Num(); i++) {
		if (Trains.IsValidIndex(i)) {
			if (IsValid(Trains[i])) {
				if (Trains[i]->IsA(ASubtrain::StaticClass())) continue;
				if (LaneValid && LaneRef->GetLaneId() != Trains[i]->GetServiceLaneId()) continue;
				double tmp = FVector::Dist(CurrentLocation, Trains[i]->GetActorLocation());
				if (Distance > tmp) {
					Distance = tmp;
					TrainIndex = i;
				}
			}
		}
	}

	if (Trains.IsValidIndex(TrainIndex)) {
		return Cast<ATrain>(Trains[TrainIndex]);
	} else {
		return nullptr;
	}
}

void ATrainManager::InitTrainMaterial() {
	TrainMaterialPath = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetTrainMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(
		TrainMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ATrainManager::TrainMaterialDeferred)
	);
}

void ATrainManager::TrainMaterialDeferred() {
	for (auto& i : TrainMaterialPath) {
		//TAssetPtr<UMaterial> tmp(i);
		TrainMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

TArray<UMaterial*> ATrainManager::GetTrainMaterial() const {
	return TrainMaterial;
}

void ATrainManager::InitPassengerMaterial() {
	PassengerMaterialPath = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetPassengerMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(
		PassengerMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ATrainManager::PassengerMaterialDeferred)
	);
}

void ATrainManager::PassengerMaterialDeferred() {
	for (auto& i : PassengerMaterialPath) {
		//TAssetPtr<UMaterial> tmp(i);
		PassengerMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

TArray<UMaterial*> ATrainManager::GetPassengerMaterial() const {
	return PassengerMaterial;
}

void ATrainManager::RefreshTrainArray() {
	for (int i = 0; i < Trains.Num(); i++) {
		if (Trains.IsValidIndex(i)) {
			if (!IsValid(Trains[i])) {
				Trains.RemoveAt(i--);
			}
		}
	}
}

void ATrainManager::TrainDeferredDespawn(FStationInfo Station, ALane* Lane) {
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->GetNextStation() == Station &&
				i->GetServiceLaneId() == Lane->GetLaneId()) {
				i->SetDespawnNextStation();
			}
		}
	}
}

TArray<ATrainTemplate*> ATrainManager::GetStationsByDestination(FStationInfo Station, ALane* Lane) {
	TArray<ATrainTemplate*> arr;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->GetNextStation() == Station &&
				i->GetServiceLaneId() == Lane->GetLaneId()) {
				arr.Add(i);
			}
		}
	}
	return arr;
}

int32 ATrainManager::GetTrainCount(int32 LaneId) const {
	int32 result = 0;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (LaneId == -1) {
				result++;
			} else {
				if (i->GetServiceLaneId() == LaneId) result++;
			}
		}
	}
	return result;
}

int32 ATrainManager::GetMainTrainCount(int32 LaneId) const {
	int32 result = 0;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->IsA(ATrain::StaticClass())) {
				if (LaneId == -1) {
					result++;
				} else {
					if (i->GetServiceLaneId() == LaneId) result++;
				}
			}
		}
	}
	return result;
}

int32 ATrainManager::GetSubTrainCount(int32 LaneId) const {
	int32 result = 0;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->IsA(ASubtrain::StaticClass())) {
				if (LaneId == -1) {
					result++;
				} else {
					if (i->GetServiceLaneId() == LaneId) result++;
				}
			}
		}
	}
	return result;
}

// Called every frame
void ATrainManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

