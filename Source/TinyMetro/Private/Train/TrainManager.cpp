// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainManager.h"
#include "Train/TrainManagerSaveGame.h"
#include "Train/TrainSpawnData.h"
#include "Train/SubtrainSpawnData.h"
#include "Train/TrainInfoWidget.h"
#include "Train/TrainTemplate.h"
#include "Train/Train.h"
#include "Train/Subtrain.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Statistics/StatisticsManager.h"
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

	TrainInfoWidgetClass = ConstructorHelpers::FObjectFinder<UClass>(TEXT("Class'/Game/Stage/UI/HUD/WBP_TrainInfoWidget.WBP_TrainInfoWidget_C'")).Object;
	TrainClass = ConstructorHelpers::FObjectFinder<UClass>(TEXT("Class'/Game/Train/BP_Train.BP_Train_C'")).Object;
	SubtrainClass = ConstructorHelpers::FObjectFinder<UClass>(TEXT("Class'/Game/Train/BP_SubTrain.BP_Subtrain_C'")).Object;

}

// Called when the game starts or when spawned
void ATrainManager::BeginPlay()
{
	Super::BeginPlay();

	// Init references
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = GameModeRef->GetStatisticsManager();

	InitTrainMaterial();
	InitPassengerMaterial();

	TrainInfoWidget = CreateWidget<UTrainInfoWidget>(GetWorld(), TrainInfoWidgetClass);
	TrainInfoWidget->AddToViewport();
	TrainInfoWidget->SetVisibility(ESlateVisibility::Hidden);

	for (auto& i : Trains) {
		i->FinishLoad();
	}

	SaveManagerRef->SaveTask.AddDynamic(this, &ATrainManager::Save);
}

void ATrainManager::AddTrain(ATrainTemplate* Train) {
	RefreshTrainArray();
	if (IsValid(Train)) {
		if (Trains.Find(Train) == INDEX_NONE) {
			Train->SetTrainId(NextTrainId++);
			Train->SetTrainInfoWidget(TrainInfoWidget);
			Trains.AddUnique(Train);
		}
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
	TArray<FSoftObjectPath> TrainMaterialPath = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetTrainMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestSyncLoad(
		TrainMaterialPath
	);
	for (auto& i : TrainMaterialPath) {
		TrainMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

TArray<UMaterial*> ATrainManager::GetTrainMaterial() const {
	return TrainMaterial;
}

void ATrainManager::InitPassengerMaterial() {
	TArray<FSoftObjectPath> PassengerMaterialPath = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode())->GetPassengerMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestSyncLoad(
		PassengerMaterialPath
	);

	for (auto& i : PassengerMaterialPath) {
		PassengerMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

TArray<UMaterial*> ATrainManager::GetPassengerMaterial() const {
	return PassengerMaterial;
}

void ATrainManager::RefreshTrainArray() {
	for (auto& i : Trains) {
		if (!IsValid(i)) {
			Trains.Remove(i);
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
	if (!TrainClass) TrainClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Class'/Game/Train/BP_Train.BP_Train_C'")));

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATrain* tmp = Cast<ATrain>(GetWorld()->SpawnActorDeferred<AActor>(TrainClass, SpawnTransform));
	tmp->SetTrainId(TrainId);
	tmp->Load();

	tmp->FinishSpawning(SpawnTransform);

	Trains.Add(MoveTemp(tmp));
}

void ATrainManager::SpawnSubtrain(int32 TrainId, int32 OwnerId, FVector SpawnLocation) {
	// Load BP Class
	if (!SubtrainClass) SubtrainClass = Cast<UClass>(StaticLoadObject(UClass::StaticClass(), NULL, TEXT("Class'/Game/Train/BP_SubTrain.BP_Subtrain_C'")));

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SpawnLocation);
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASubtrain* tmp = Cast<ASubtrain>(GetWorld()->SpawnActorDeferred<AActor>(SubtrainClass, SpawnTransform));
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

void ATrainManager::ReleaseClick() {
	if (IsValid(ClickedTrain)) ClickedTrain->OnReleasedLogic();
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
	
	TMap<int, int> trainCountInLane = {
		{1,0},
		{2,0},
		{3,0},
		{4,0},
		{5,0},
		{6,0},
		{7,0},
		{8,0}
	};

	for (auto& i : Trains) {
		int serviceLaneId = i->GetTrainInfo().ServiceLaneId;
		if (trainCountInLane.Contains(serviceLaneId)) {
			trainCountInLane[serviceLaneId]++;
		}
	}

	for (auto& i : trainCountInLane) {
		StatisticsManagerRef->LaneStatistics.Lanes[i.Key].ServiceTrainAndSubtrainCount = i.Value;
	}
}

