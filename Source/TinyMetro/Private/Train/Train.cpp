// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"
#include "Train/TrainManager.h"
#include "Train/SubtrainAiController.h"
#include "Station/Station.h"
#include "Station/PathQueue.h"
#include "Lane/Lane.h"
#include "Lane/LaneManager.h"
#include "Components/BoxComponent.h"
#include "GameModes/GameModeBaseSeoul.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Engine/AssetManager.h>

void ATrain::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		TEXT("Train::Tick")
	);*/

	// Passenger Off, On
	if (Status == TrainStatus::GetOff) {
		PassengerTransitionCount += DeltaTime;
		if (PassengerTransitionCount >= PassengerTransitionDelay) {
			bool tmp;
			GetOffPassenger(StationManagerRef->GetStationByStationInfo(CurrentStation), tmp);
			PassengerTransitionCount -= PassengerTransitionDelay;
		}
	} else if (Status == TrainStatus::GetOn) {
		PassengerTransitionCount += DeltaTime;
		if (PassengerTransitionCount >= PassengerTransitionDelay) {
			GetOnPassenger(StationManagerRef->GetStationByStationInfo(CurrentStation));
			PassengerTransitionCount -= PassengerTransitionDelay;
		}
	}

	// Drag & Drop
	if (IsActorDragged) {
		//DropPassenger();
		//MouseToWorldLocation;
		//MouseToWorldActor = ConvertMousePositionToWorldLocation(MouseToWorldLocation);

		///*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		//	FString::Printf(TEXT("Train::Tick - %lf, %lf"), MouseToWorldLocation.X, MouseToWorldLocation.Y));*/
		//LaneRef = Cast<ALane>(MouseToWorldActor);
		//SetTrainMaterial(LaneRef);
		//if (MouseToWorldActor->IsA(AStation::StaticClass()) ||
		//	MouseToWorldActor->IsA(ATrainTemplate::StaticClass())) {
		//	LineTraceIgnoreActors.AddUnique(MouseToWorldActor);
		//}
		//if (IsValid(LaneRef)) {
		//	
		//}

		Destination = StationManagerRef->GetNearestStation(MouseToWorldLocation, LaneRef);
		this->SetActorLocationAndRotation(
			MouseToWorldLocation,
			FRotator(
				0,
				UKismetMathLibrary::FindLookAtRotation(MouseToWorldLocation, Destination->GetActorLocation()).Yaw,
				0
			)
		);

		FVector TrainForwardVector = TrainMeshComponent->GetForwardVector();
		FRotator TrainRotationVector = TrainMeshComponent->GetComponentRotation();
		for (int i = 0; i < Subtrains.Num(); i++) {
			FVector NewSubtrainLocation = TrainForwardVector * TrainSafeDistance * (i + 1);
			Subtrains[i]->SetActorLocationAndRotation(NewSubtrainLocation, TrainRotationVector);
			//FRotator TrainWorldRotation = TrainMeshComponent->getrotationo
		}
	}
}

void ATrain::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Train"));
}

ATrain::ATrain() {
	

	// Load meshes (Train)
	for (auto& i : TrainMeshPath) {
		TrainMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}

	/*ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_Train.SM_Train'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);*/
	
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	OverlapVolume->InitBoxExtent(FVector(10,20,30));
	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnOverlapBegin);
	OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ATrain::OnOverlapEnd);
	OverlapVolume->SetupAttachment(RootComponent);

	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(TrainMesh[0]);
	TrainMeshComponent->SetupAttachment(RootComponent);



	UpdatePassengerSlot();

	CubeComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
	CubeComponent->SetupAttachment(GetRootComponent());
}

void ATrain::BeginPlay() {
	/*PassengerMeshPosition = {
		FVector(10.0f, 55.0f, 190.0f),
		FVector(10.0f, -55.0f, 190.0f),
		FVector(-100.0f, 55.0f, 190.0f),
		FVector(-100.0f, -55.0f, 190.0f),
		FVector(-210.0f, 55.0f, 190.0f),
		FVector(-210.0f, -55.0f, 190.0f),
		FVector(-320.0f, 55.0f, 190.0f),
		FVector(-320.0f, -55.0f, 190.0f)
	};
	PassengerMeshPositionUpgrade = {
		FVector(70.0f, 55.0f, 190.0f),
		FVector(70.0f, -55.0f, 190.0f),
		FVector(-50.0f, 55.0f, 190.0f),
		FVector(-50.0f, -55.0f, 190.0f),
		FVector(-160.0f, 55.0f, 190.0f),
		FVector(-160.0f, -55.0f, 190.0f),
		FVector(-270.0f, 55.0f, 190.0f),
		FVector(-270.0f, -55.0f, 190.0f)
	};*/
	Super::BeginPlay();
	TotalTravel = 0.0f;
	AiControllerRef = Cast<ATrainAiController>(GetController());
}

void ATrain::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AStation::StaticClass()) && !IsActorDragged) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Overlap"));
		
		auto Station = Cast<AStation>(OtherActor);
		// Check passing station
		if (Station->GetLanes().Contains(ServiceLaneId)) {

			if (DeferredDespawn) {
				for (auto& i : Subtrains) {
					i->DespawnTrain();
				}
				DespawnTrain();
				return;
			}

			// Set current, next Station
			SetCurrentStation(Station->GetStationInfo());
			SetNextStation(LaneManagerRef->GetLaneById(ServiceLaneId)->GetNextStation(
					Station,
					GetTrainDirection()
				)->GetStationInfo()
			);
			
			// Set subtrain's current, next station
			for (auto& i : Subtrains) {
				i->SetCurrentStation(GetCurrentStation());
				i->SetNextStation(GetNextStation());
			}

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Overlap"));

			// Movement stop, release
			TrainMovement->SetActive(false);

			// Initialize check index
			PassengerIndex = 0;

			// Change status : Run -> GetOff
			Status = TrainStatus::GetOff;

			//bool tmp = false;
			//// Set up delegate (Passenger get on, off)
			//GetOffDelegate.BindUObject(
			//	this,
			//	&ATrain::GetOffPassenger,
			//	Cast<AStation>(OtherActor),
			//	&tmp
			//);

			//GetOnDelegate.BindUObject(
			//	this,
			//	&ATrain::GetOnPassenger,
			//	Cast<AStation>(OtherActor)
			//);

			//// Start get off passenger
			//GetWorld()->GetTimerManager().SetTimer(
			//	GetOffHandle,
			//	GetOffDelegate,
			//	1.0f,
			//	true,
			//	0.0f
			//);

			AiControllerRef->SetTrainDestination(
				GetNextTrainDestination(AiControllerRef->GetTrainDestination())
			);
		}
	}
	
	
}

void ATrain::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	/*if (OtherActor->IsA(AStation::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("End Overlap"));
	}*/
}

FVector ATrain::GetNextTrainDestination(FVector CurLocation) {
	//UE_LOG(LogTemp, Log, TEXT("Train::GetNextTrainDestination"));
	bool tmp;
	auto LaneTmp = LaneManagerRef->GetLaneById(ServiceLaneId);
	auto NextPoint = LaneTmp->GetNextLocation(
		this,
		GridManagerRef->GetGridCellDataByCoord(CurLocation, tmp).WorldCoordination,
		Direction
	);

	return GridManagerRef->GetGridCellDataByPoint(NextPoint.X, NextPoint.Y).WorldLocation;
}

void ATrain::SetTrainMaterial(ALane* Lane) {
	Super::SetTrainMaterial(Lane);
	/*if (IsValid(Lane)) {
		TrainMeshComponent->SetMaterial(0, TrainMaterial[Lane->GetLaneId()]);
		for (auto& i : PassengerMeshComponent) {
			i->SetMaterial(0, PassengerMaterial[Lane->GetLaneId()]);
		}
	} else {
		TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	}*/
}

bool ATrain::IsPassengerSlotFull() {
	int32 ValidSeat = GetValidSeatCount();
	for (auto& i : Subtrains) {
		if (ValidSeat > 0) {
			break;
		}
		ValidSeat += i->GetValidSeatCount();
	}

	if (ValidSeat <= 0) { // Seat full
		return true;
	} else { // Seat not full
		return false;
	}
}

void ATrain::TrainOnReleased(AActor* Target, FKey ButtonPressed) {
	Super::TrainOnReleased(Target, ButtonPressed);

	// Drag & Drop operation
	if (!IsSingleClick) {
		if (IsValid(LaneRef)) {
			/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
				FString::Printf(TEXT("Train::Release before - %lf, %lf"), this->GetActorLocation().X, this->GetActorLocation().Y));*/
			FVector StartLocation = GridManagerRef->Approximate(
				this->GetActorLocation(),
				LaneRef->GetLaneShape(this->GetActorLocation())
			);
			/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
				FString::Printf(TEXT("Train::Release after - %lf, %lf"), StartLocation.X, StartLocation.Y));*/

			SetActorLocation(StartLocation);
			ServiceStart(StartLocation, LaneRef, Destination);
		} else {
			// TODO : if upgrade, return upgrade cost
			DespawnTrain();
		}
	}
	
	// Reset single click checker
	IsSingleClick = false;
}

void ATrain::ServiceStart(FVector StartLocation, ALane* Lane, class AStation* D) {
	Super::ServiceStart(StartLocation, Lane, D);
	bool tmp;

	StartLocation.Z = 20.f;
	this->SetActorLocation(StartLocation);

	// Set serviced lane id
	SetServiceLaneId(Lane->GetLaneId());
	// Set train direction (Down or Up)
	SetTrainDirection(Lane->SetDirectionInit(
		D,
		GridManagerRef->GetGridCellDataByCoord(StartLocation, tmp).WorldCoordination
	));

	// Set train destination (Next grid)
	AiControllerRef->SetTrainDestination(GetNextTrainDestination(StartLocation));

	// Initialize train's Current, Next station
	CurrentStation.Id = -1;
	NextStation = D->GetStationInfo();

	// Set train material
	LaneRef = Lane;
	SetTrainMaterial(LaneRef);
	

	// Train move start
	AiControllerRef->Patrol();

	FVector cubeLocation = this->GetActorLocation();
	cubeLocation.Z = cubeLocation.Z + 300.0f;
	CubeComponent->SetWorldLocation(cubeLocation);
}

void ATrain::UpdatePassengerSlot() {
	Super::UpdatePassengerSlot();
	if (IsUpgrade) {
		for (int i = 0; i < PassengerMeshPositionUpgrade.Num(); i++) {
			PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPositionUpgrade[i]);
		}
	} else {
		for (int i = 0; i < PassengerMeshPosition.Num(); i++) {
			PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPosition[i]);
		}
	}
}

void ATrain::DespawnTrain() {
	PlayerStateRef->AddItem(ItemType::Train, 1);
	for (auto& i : Subtrains) {
		i->DespawnTrain();
	}
	Super::DespawnTrain();
}

void ATrain::UpdateTrainMesh() {
	Super::UpdateTrainMesh();
	TrainMeshComponent->SetStaticMesh(TrainMesh[IsUpgrade]);
	SetTrainMaterial(LaneRef);
}

void ATrain::Upgrade() {
	Super::Upgrade();
	// TODO : Money function division
	PlayerStateRef->AddMoney(-TrainManagerRef->GetCostUpgradeTrain());
	TrainManagerRef->ReportTrainUpgrade();
	UpdateSubtrainSpeed();
	UpdateSubtrainDistance();
	for (auto& i : Subtrains) {
		i->UpdateTrainMesh();
	}
}

bool ATrain::CanUpgrade() const {
	if (PlayerStateRef->GetMoney() < TrainManagerRef->GetCostUpgradeTrain()) {
		return false;
	} else {
		return true;
	}
}

void ATrain::ActiveMoveTest() {
	TrainMovement->SetActive(true);
}

void ATrain::GetOnPassenger(AStation* Station) {
	if (!IsPassengerSlotFull()) {
		// Log
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Staton::GetOnPassenger"));

		auto RidePassenger = Station->GetOnPassenger(PassengerIndex++, this);

		if (RidePassenger.Destination != StationType::None) {
			PassengerIndex--;
			// Add Passenger
			if (!AddPassenger(RidePassenger)) { // Train is full
				// Passenger get on subtrain
				for (auto& i : Subtrains) {
					if (i->AddPassenger(RidePassenger)) {
						break;
					}
				}
			} 
		} else {
			//GetWorld()->GetTimerManager().ClearTimer(GetOnHandle);
			Status = TrainStatus::Run;
			TrainMovement->SetActive(true);

			AiControllerRef->Patrol();
		}
	} else {
		//GetWorld()->GetTimerManager().ClearTimer(GetOnHandle);
		Status = TrainStatus::Run;
		TrainMovement->SetActive(true);

		AiControllerRef->Patrol();
	}
}

void ATrain::GetOffPassenger(AStation* Station, bool& Success) {
	// Log
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Station::GerOffPassenger"));
	
	Super::GetOffPassenger(Station, Success);
	if (Success) {
		return;
	}

	for (auto& i : Subtrains) {
		i->GetOffPassenger(Station, Success);
		if (Success) {
			return;
		}
	}
	
	// Call when any passenger get off
	// Stop get off, Start get on
	/*GetWorld()->GetTimerManager().ClearTimer(GetOffHandle);
	GetWorld()->GetTimerManager().SetTimer(
		GetOnHandle,
		GetOnDelegate,
		1.0f,
		true,
		0.0f
	);*/
	Status = TrainStatus::GetOn;
}

void ATrain::UpdateSubtrainDistance() {
	int count = 1;
	// Distance between Train and 1st Subtrain
	float defaultDistance = 0.0f;
	// Is before Subtrain is upgrade
	bool isUpgradeBeforeSubtrain = false;
	// Total distance from Train to last Subtrain
	float totalDistance = 0.0f;
	if (Subtrains.IsValidIndex(0)) {
		if (IsUpgrade) {
			if (Subtrains[0]->GetIsUpgrade()) {
				// Upgrade Train and Upgrade Subtrain
				defaultDistance = DIST_UP_TRAIN_UP_SUBTRAIN;
				isUpgradeBeforeSubtrain = true;
			} else {
				// Upgrade Train and Non upgrade Subtrain
				defaultDistance = DIST_UP_TRAIN_SUBTRAIN;
				isUpgradeBeforeSubtrain = false;
			}
		} else {
			if (Subtrains[0]->GetIsUpgrade()) {
				// Non upgrade Train and Upgrade Subtrain
				defaultDistance = DIST_TRAIN_UP_SUBTRAIN;
				isUpgradeBeforeSubtrain = true;
			} else {
				// Non upgrade Train and Non upgrade Subtrain
				defaultDistance = DIST_TRAIN_SUBTRAIN;
				isUpgradeBeforeSubtrain = false;
			}
		}
		Subtrains[0]->SetDistanceFromTrain(defaultDistance);
		totalDistance += defaultDistance;
	}

	if (Subtrains.Num() >= 2) {
		for (int i = 1; i < Subtrains.Num(); i++) {
			if (Subtrains[i]->GetIsUpgrade()) {
				if (isUpgradeBeforeSubtrain) {
					// Upgrade Subtrain and Upgrade Subtrain
					Subtrains[i]->SetDistanceFromTrain(totalDistance + DIST_UP_SUBTRAIN_UP_SUBTRAIN);
					totalDistance += DIST_UP_SUBTRAIN_UP_SUBTRAIN;
				} else {
					// Non upgrade Subtrain and Upgrade Subtrain
					Subtrains[i]->SetDistanceFromTrain(totalDistance + DIST_SUBTRAIN_UP_SUBTRAIN);
					totalDistance += DIST_SUBTRAIN_UP_SUBTRAIN;
				}
			} else {
				if (isUpgradeBeforeSubtrain) {
					// Upgrade Subtrain and Non upgrade Subtrain
					Subtrains[i]->SetDistanceFromTrain(totalDistance + DIST_UP_SUBTRAIN_SUBTRAIN);
					totalDistance += DIST_UP_SUBTRAIN_SUBTRAIN;
				} else {
					// Non upgrade Subtrain and Non upgrade Subtrain
					Subtrains[i]->SetDistanceFromTrain(totalDistance + DIST_SUBTRAIN_SUBTRAIN);
					totalDistance += DIST_SUBTRAIN_SUBTRAIN;
				}
			}
		}
	}
}

void ATrain::DetachSubtrain(ASubtrain* T) {
	Subtrains.Remove(T);
	UpdateSubtrainDistance();
}

void ATrain::UpdateSubtrainSpeed() {
	for (auto& i : Subtrains) {
		i->SetTrainSpeed(IsUpgrade ? TRAIN_UPGRADE_SPEED : TRAIN_DEFAULT_SPEED);
	}
}

void ATrain::AddSubtrain(ASubtrain* T) {
	Cast<ASubtrainAiController>(T->GetController())->SetTargetTrain(this);
	Subtrains.AddUnique(T);
	T->AttachToTrain(this);
	T->SetActorLocation(this->GetActorLocation());
	UpdateSubtrainDistance();
	UpdateSubtrainSpeed();
}

