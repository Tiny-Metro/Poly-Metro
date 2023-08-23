// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainManager.h"
#include "Train/TrainManagerSaveGame.h"
#include "Train/TrainSpawnData.h"
#include "Train/SubtrainSpawnData.h"
#include "Train/TrainInfoWidget.h"
#include "Train/Train.h"
#include "Train/Subtrain.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "SaveSystem/TMSaveManager.h"
#include "Lane/Lane.h"
#include <Blueprint/UserWidget.h>
#include "GameModes/TinyMetroGameModeBase.h"
#include <Kismet/GameplayStatics.h>
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

	// Init references
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();

	InitTrainMaterial();
	InitPassengerMaterial();

	FSoftClassPath MyWidgetClassRef(TEXT("Blueprint'/Game/Stage/UI/HUD/WBP_TrainInfoWidget.WBP_TrainInfoWidget_C'"));
	if (UClass* MyWidgetClass = MyWidgetClassRef.TryLoadClass<UUserWidget>()) {
		TrainInfoWidget = CreateWidget<UTrainInfoWidget>(GetWorld(), MyWidgetClass);
		TrainInfoWidget->AddToViewport();
		TrainInfoWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	SaveManagerRef->SaveTask.AddDynamic(this, &ATrainManager::Save);
}

void ATrainManager::AddTrain(ATrainTemplate* Train) {
	RefreshTrainArray();
	if (Trains.Find(Train) == INDEX_NONE) {
		Train->SetTrainId(NextTrainId++);
		Train->SetTrainInfoWidget(TrainInfoWidget);
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

TArray<ATrainTemplate*> ATrainManager::GetAllTrain() {
	return Trains;
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

int32 ATrainManager::GetTrainCountFilterByUpgrade(bool Upgrade, int32 LaneId) const {
	int32 result = 0;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (LaneId == -1 && (i->GetIsUpgrade() == Upgrade)) {
				result++;
			} else {
				if (i->GetServiceLaneId() == LaneId && (i->GetIsUpgrade() == Upgrade)) result++;
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

int32 ATrainManager::GetMainTrainCountFilterByUpgrade(bool Upgrade, int32 LaneId) const {
	int32 result = 0;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->IsA(ATrain::StaticClass())) {
				if (LaneId == -1 && (i->GetIsUpgrade() == Upgrade)) {
					result++;
				} else {
					if (i->GetServiceLaneId() == LaneId && (i->GetIsUpgrade() == Upgrade)) result++;
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

int32 ATrainManager::GetSubTrainCountFilterByUpgrade(bool Upgrade, int32 LaneId) const {
	int32 result = 0;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->IsA(ASubtrain::StaticClass())) {
				if (LaneId == -1 && (i->GetIsUpgrade() == Upgrade)) {
					result++;
				} else {
					if (i->GetServiceLaneId() == LaneId && (i->GetIsUpgrade() == Upgrade)) result++;
				}
			}
		}
	}
	return result;
}

void ATrainManager::SpawnTrain(int32 TrainId, FVector SpawnLocation) {
	// Load BP Class
	if (!TrainBlueprintClass) TrainBlueprintClass = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Train/BP_Train.BP_Train'")));
	// Cast to BP
	if (!GeneratedTrainBlueprint) GeneratedTrainBlueprint = Cast<UBlueprint>(TrainBlueprintClass);

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATrain* tmp = Cast<ATrain>(GetWorld()->SpawnActorDeferred<AActor>(GeneratedTrainBlueprint->GeneratedClass, SpawnTransform));
	tmp->SetTrainId(TrainId);
	tmp->Load();

	tmp->FinishSpawning(SpawnTransform);

	Trains.Add(MoveTemp(tmp));
}

void ATrainManager::SpawnSubtrain(int32 TrainId, int32 OwnerId, FVector SpawnLocation) {
	// Load BP Class
	if (!SubtrainBlueprintClass) SubtrainBlueprintClass = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Train/BP_SubTrain.BP_Subtrain'")));
	// Cast to BP
	if (!GeneratedSubtrainBlueprint) GeneratedSubtrainBlueprint = Cast<UBlueprint>(SubtrainBlueprintClass);

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASubtrain* tmp = Cast<ASubtrain>(GetWorld()->SpawnActorDeferred<AActor>(GeneratedSubtrainBlueprint->GeneratedClass, SpawnTransform));
	tmp->SetTrainId(TrainId);
	tmp->SetOwnerTrainId(OwnerId);
	tmp->Load();

	tmp->FinishSpawning(SpawnTransform);

	Trains.Add(MoveTemp(tmp));
}

float ATrainManager::GetCostUpgradeTrain() const {
	return CostUpgradeTrain;
}

float ATrainManager::GetCostUpgradeSubtrain() const {
	return CostUpgradeSubtrain;
}

// If something change when Train upgrade, imple init
// Only after upgrade
// Not imple use money for upgrade
void ATrainManager::ReportTrainUpgrade() {
	UE_LOG(LogTemp, Log, TEXT("TrainManager::ReportTrainUprade : Train Upgrade"))
}

// If something change when Subtrain upgrade, imple init
// Only after upgrade
// Not imple use money for upgrade
void ATrainManager::ReportSubtrainUpgrade() {
	UE_LOG(LogTemp, Log, TEXT("TrainManager::ReportSubtrainUprade : Subtrain Upgrade"))
}

void ATrainManager::Save() {
	UTrainManagerSaveGame* tmp = Cast<UTrainManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(UTrainManagerSaveGame::StaticClass()));

	tmp->NextTrainId = NextTrainId;
	tmp->CostUpgradeTrain = CostUpgradeTrain;
	tmp->CostUpgradeSubtrain = CostUpgradeSubtrain;
	for (auto& i : Trains) {
		auto trainInfo = i->GetTrainInfo();
		if (trainInfo.Type == TrainType::Train) {
			FTrainSpawnData trainSpawnData;
			trainSpawnData.Id = trainInfo.Id;
			trainSpawnData.Location = i->GetActorLocation();

			tmp->TrainSapwnData.Add(trainSpawnData);
		} else {
			FSubtrainSpawnData subtrainSpawnData;
			subtrainSpawnData.Id = trainInfo.Id;
			subtrainSpawnData.Location = i->GetActorLocation();
			
			tmp->SubtrainSapwnData.Add(subtrainSpawnData);
		}
	}

	SaveManagerRef->Save(tmp, SaveActorType::TrainManager);
}

bool ATrainManager::Load() {
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	UTrainManagerSaveGame* tmp = Cast<UTrainManagerSaveGame>(SaveManagerRef->Load(SaveActorType::TrainManager));

	if (!IsValid(tmp)) {
		return false;
	}

	NextTrainId = tmp->NextTrainId;
	CostUpgradeTrain = tmp->CostUpgradeTrain;
	CostUpgradeSubtrain = tmp->CostUpgradeSubtrain;

	for (auto& i : tmp->TrainSapwnData) {
		SpawnTrain(i.Id, i.Location);
	}

	for (auto& i : tmp->SubtrainSapwnData) {
		SpawnSubtrain(i.Id, i.OwnTrainId, i.Location);
	}

	return true;
}

int32 ATrainManager::GetStationCountByOrigin(FStationInfo Origin, ALane* Lane) {
	TArray<ATrainTemplate*> arr;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if ((i->GetCurrentStation() == Origin &&
				i->GetServiceLaneId() == Lane->GetLaneId()) || i->GetCurrentStation().Id == -1) {
				arr.Add(i);
			}
		}
	}
	return arr.Num();
}

int32 ATrainManager::GetStationCountByDestination(FStationInfo Destination, ALane* Lane) {
	TArray<ATrainTemplate*> arr;
	for (auto& i : Trains) {
		if (IsValid(i)) {
			if (i->GetNextStation() == Destination &&
				i->GetServiceLaneId() == Lane->GetLaneId()) {
				arr.Add(i);
			}
		}
	}
	return arr.Num();

}

// Called every frame
void ATrainManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

