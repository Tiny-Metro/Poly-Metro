// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Subtrain.h"
#include "Train/Train.h"
#include "Train/TrainManager.h"
#include "Train/SubtrainAiController.h"
#include "Train/SubtrainSaveGame.h"
#include "Lane/Lane.h"
#include "Lane/LaneManager.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Statistics/StatisticsManager.h"
#include "SaveSystem/TMSaveManager.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

ASubtrain::ASubtrain() {

	// Load meshes (Subtrain)
	for (auto& i : SubtrainMeshPath) {
		SubtrainMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}

	// Load meshes (UpgradeSubtrain)
	for (auto& i : UpgradeSubtrainMeshPath) {
		UpgradeSubtrainMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}

	/*static ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_SubtrainShort.SM_SubtrainShort'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);*/

	TrainMovement->MaxWalkSpeed = 350.0f; // Default 600
	TrainMovement->MaxAcceleration = 3000.0f; // Default 2048

	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(SubtrainMesh[0]);
	//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	//TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);



	UpdatePassengerSlot();
}

void ASubtrain::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// Drag activated
	if (IsActorDragged) {
		DetachFromTrain();
		//DropPassenger();
		//MouseToWorldLocation;
		//MouseToWorldActor = ConvertMousePositionToWorldLocation(MouseToWorldLocation);

		///*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		//	FString::Printf(TEXT("Train::Tick - %lf, %lf"), MouseToWorldLocation.X, MouseToWorldLocation.Y));*/
		//LaneRef = Cast<ALane>(MouseToWorldActor);
		//SetTrainMaterial(LaneRef);
		//if (MouseToWorldActor->IsA(AStation::StaticClass())) {
		//	LineTraceIgnoreActors.AddUnique(MouseToWorldActor);
		//}
		//if (IsValid(LaneRef)) {

		//}

		OwnerTrainRef = TrainManagerRef->GetNearestTrain(MouseToWorldLocation, LaneRef);
		if (IsValid(OwnerTrainRef)) {
			this->SetActorLocationAndRotation(
				MouseToWorldLocation,
				FRotator(
					0,
					UKismetMathLibrary::FindLookAtRotation(MouseToWorldLocation, OwnerTrainRef->GetActorLocation()).Yaw,
					0
				)
			);
		}
	}
}

void ASubtrain::Test() {

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Subtrain"));
}

void ASubtrain::SetTrainMaterial(ALane* Lane) {
	Super::SetTrainMaterial(Lane);
}

void ASubtrain::UpdatePassengerSlot() {
	Super::UpdatePassengerSlot();
	if (TrainInfo.IsUpgrade) {
		for (int i = 0; i < PassengerMeshPositionUpgrade.Num(); i++) {
			PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPositionUpgrade[i]);
		}
	} else {
		for (int i = 0; i < PassengerMeshPosition.Num(); i++) {
			PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPosition[i]);
		}
	}
}

void ASubtrain::DespawnTrain() {
	PlayerStateRef->AddItem(ItemType::Subtrain, 1);
	Super::DespawnTrain();
}

void ASubtrain::UpdateTrainMesh() {
	Super::UpdateTrainMesh();
	if (IsValid(OwnerTrainRef)) {
		if (OwnerTrainRef->GetIsUpgrade()) {
			TrainMeshComponent->SetStaticMesh(UpgradeSubtrainMesh[TrainInfo.IsUpgrade]);
		} else {
			TrainMeshComponent->SetStaticMesh(SubtrainMesh[TrainInfo.IsUpgrade]);
		}
	} else {
		TrainMeshComponent->SetStaticMesh(SubtrainMesh[TrainInfo.IsUpgrade]);
	}
}

void ASubtrain::Upgrade() {
	if (!TrainInfo.IsUpgrade) {
		if (PlayerStateRef->CanUseMoney(TrainManagerRef->GetCostUpgradeSubtrain())) {
			PlayerStateRef->AddMoney(-TrainManagerRef->GetCostUpgradeSubtrain());
			StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalUpgradeCount++;
		} else {
			return;
		}
	}

	Super::Upgrade();
	TrainManagerRef->ReportSubtrainUpgrade();
}

bool ASubtrain::CanUpgrade() const {
	if (PlayerStateRef->GetMoney() < TrainManagerRef->GetCostUpgradeSubtrain()) {
		return false;
	} else {
		return true;
	}
}

void ASubtrain::TrainOnReleased(AActor* Target, FKey ButtonPressed) {
	Super::TrainOnReleased(Target, ButtonPressed);

	// Drag & Drop operation
	if (!IsSingleClick) {
		if (IsValid(LaneRef)) {
			FVector StartLocation = GridManagerRef->Approximate(GetActorLocation(), LaneRef->GetLaneShape(GetActorLocation()));
			StartLocation.Z = 20.0f;
			SetActorLocation(StartLocation);
			OwnerTrainRef->AddSubtrain(this);
			ServiceStart(GetActorLocation(), LaneRef, nullptr);
			StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalShiftCount++;
		} else {
			StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalRetrievalCount++;
			DespawnTrain();
		}
	}

	// Reset single click checker
	IsSingleClick = false;
}

void ASubtrain::BeginPlay() {
	Super::BeginPlay();
	if(!IsValid(AiControllerRef)) AiControllerRef = Cast<ASubtrainAiController>(GetController());
	TrainInfo.Type = TrainType::SubTrain;
}

//FVector ASubtrain::GetNextTrainPosition() {
//	return FVector();
//}

void ASubtrain::ServiceStart(FVector StartLocation, ALane* Lane, AStation* D) {
	LaneRef = Lane;
	Super::ServiceStart(StartLocation, LaneRef, D);
	SetTrainMaterial(LaneRef);
}

void ASubtrain::SetOwnerTrainId(int32 TID) {
	OwnerTrainId = TID;
}

int32 ASubtrain::GetOwnerTrainId() const {
	return OwnerTrainId;
}

void ASubtrain::SetDistanceFromTrain(float Dist) {
	DistanceFromTrain = Dist;

	if (!IsValid(AiControllerRef)) AiControllerRef = Cast<ASubtrainAiController>(GetController());
	AiControllerRef->SetDistanceFromTrain(Dist);
}

void ASubtrain::DetachFromTrain() {
	IsAttached = false;
	if (IsValid(OwnerTrainRef)) {
		OwnerTrainRef->DetachSubtrain(this);
	}
	OwnerTrainRef = nullptr;
	OwnerTrainId = -1;
}

void ASubtrain::AttachToTrain(ATrain* Train) {
	IsAttached = true;
	OwnerTrainRef = Train;
	OwnerTrainId = Train->GetTrainId();

	if (!IsValid(AiControllerRef)) AiControllerRef = Cast<ASubtrainAiController>(GetController());
	AiControllerRef->SetTargetTrain(Train);
}

void ASubtrain::SetIndex(int32 Idx) {
	Index = Idx;
}

int32 ASubtrain::GetIndex() const {
	return Index;
}

void ASubtrain::WeeklyTask() {
	UE_LOG(LogTemp, Log, TEXT("Subtrain::WeeklyTask"));
}

void ASubtrain::Save() {
	Super::Save();
	USubtrainSaveGame* tmp = Cast<USubtrainSaveGame>(UGameplayStatics::CreateSaveGameObject(USubtrainSaveGame::StaticClass()));
	
	tmp->TrainInfo = TrainInfo;
	tmp->Passenger = Passenger;
	tmp->OwnerTrainId = OwnerTrainId;

	LaneRef = LaneManagerRef->Lanes[TrainInfo.ServiceLaneId];

	SaveManagerRef->Save(tmp, SaveActorType::Train, TrainInfo.Id);
}

bool ASubtrain::Load() {
	Super::Load();
	USubtrainSaveGame* tmp = Cast<USubtrainSaveGame>(SaveManagerRef->Load(SaveActorType::Train, TrainInfo.Id));

	// Load fail
	if (!IsValid(tmp)) return false;

	TrainInfo = tmp->TrainInfo;
	Passenger = tmp->Passenger;
	OwnerTrainId = tmp->OwnerTrainId;
	LaneRef = LaneManagerRef->Lanes[TrainInfo.ServiceLaneId];

	return true;
}

void ASubtrain::FinishLoad() {
	if (IsLoaded) {
		Super::FinishLoad();
		if (IsValid(LaneRef)) {
			UE_LOG(LogTemp, Log, TEXT("Subtrain::FinishLoad - Lane Valid"));
			TrainType tmp;
			OwnerTrainRef = Cast<ATrain>(TrainManagerRef->GetTrainById(OwnerTrainId, tmp));
			if (!IsValid(OwnerTrainRef)) DespawnTrain();
			OwnerTrainRef->AddSubtrain(this);
			ServiceStart(GetActorLocation(), LaneRef, nullptr);
			UpdatePassengerMesh();
		}
		if (TrainInfo.IsUpgrade) {
			Upgrade();
		}
	} else {
		DespawnTrain();
	}
}
