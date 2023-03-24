// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"
#include "Train/SubtrainAiController.h"
#include "Station/Station.h"
#include "Station/PathQueue.h"
#include "Lane/Lane.h"
#include "Lane/LaneManager.h"
#include "Components/BoxComponent.h"
#include "GameModes/GameModeBaseSeoul.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/AssetManager.h>

void ATrain::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		TEXT("Train::Tick")
	);*/

	if (IsActorDragged) {
		FVector MouseToWorldLocation;
		AActor* MouseToWorldActor = ConvertMousePositionToWorldLocation(MouseToWorldLocation);
		this->SetActorLocation(MouseToWorldLocation);

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
	ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_Train.SM_Train'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);
	
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	OverlapVolume->InitBoxExtent(FVector(10,20,30));
	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnOverlapBegin);
	OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ATrain::OnOverlapEnd);
	OverlapVolume->SetupAttachment(RootComponent);

	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(LoadTrainMesh.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);
	

	for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPosition[i]);
	}
}

void ATrain::BeginPlay() {
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
			// Set current Station
			CurrentStation = Station->GetStationInfo();
			// Set next Station
			NextStation = LaneManagerRef->GetLaneById(ServiceLaneId)->GetNextStation(
				Station,
				GetTrainDirection()
			)->GetStationInfo();

			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Overlap"));

			// Movement stop, release
			TrainMovement->SetActive(false);

			// Initialize check index
			PassengerIndex = 0;

			// Set up delegate (Passenger get on, off)
			GetOffDelegate.BindUObject(
				this,
				&ATrain::GetOffPassenger,
				Cast<AStation>(OtherActor)
			);

			GetOnDelegate.BindUObject(
				this,
				&ATrain::GetOnPassenger,
				Cast<AStation>(OtherActor)
			);

			// Start get off passenger
			GetWorld()->GetTimerManager().SetTimer(
				GetOffHandle,
				GetOffDelegate,
				1.0f,
				true,
				0.0f
			);

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
	if (IsValid(Lane)) {
		TrainMeshComponent->SetMaterial(0, TrainMaterial[Lane->GetLaneId()]);
		for (auto& i : PassengerMeshComponent) {
			i->SetMaterial(0, PassengerMaterial[Lane->GetLaneId()]);
		}
	} else {
		TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	}
}

void ATrain::Upgrade() {
	// Change mesh
	TrainMeshComponent->SetStaticMesh(TrainMesh[0]);
	// Set flag
	IsUpgrade = true;
	// Set passenger slot
	CurrentPassengerSlot = MaxPassengerSlotUpgrade;
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

void ATrain::ServiceStart(FVector StartLocation, ALane* Lane, class AStation* Destination) {
	bool tmp;

	// Set serviced lane id
	SetServiceLaneId(Lane->GetLaneId());
	// Set train direction (Down or Up)
	SetTrainDirection(Lane->SetDirectionInit(
		Destination,
		GridManagerRef->GetGridCellDataByCoord(StartLocation, tmp).WorldCoordination
	));

	// Set train destination (Next grid)
	AiControllerRef->SetTrainDestination(GetNextTrainDestination(StartLocation));

	// Initialize train's Current, Next station
	CurrentStation.Id = -1;
	NextStation = Destination->GetStationInfo();

	// Train move start
	AiControllerRef->Patrol();
}

void ATrain::ActiveMoveTest() {
	TrainMovement->SetActive(true);
}

void ATrain::GetOnPassenger(AStation* Station) {
	if (!IsPassengerSlotFull()) {
		// Log
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Staton::GetOnPassenger"));


		auto RidePassenger = Station->GetOnPassenger(PassengerIndex++, this);


		if (RidePassenger != nullptr) {
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
			TotalPassenger++;

			// Get money
			PlayerStateRef->AddMoney(Fare);
			UpdatePassengerMesh();
		} else {
			GetWorld()->GetTimerManager().ClearTimer(GetOnHandle);
			TrainMovement->SetActive(true);
		}
	} else {
		GetWorld()->GetTimerManager().ClearTimer(GetOnHandle);
		TrainMovement->SetActive(true);
	}
}

void ATrain::GetOffPassenger(AStation* Station) {
	// Log
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Station::GerOffPassenger"));

	for (int i = 0; i < CurrentPassengerSlot; i++) {
		if (Passenger[i]) {
			// Update passenger route
			auto PassengerRoute = StationManagerRef->GetShortestPath(
				CurrentStation.Id,
				Passenger[i]->GetDestination()
			);

			//auto PassengerRoute = Passenger[i]->GetPassengerPath();
			// Check route validation
			// True : Route valid
			// False(else) : Route invalid (Get off)
			if (!PassengerRoute.IsEmpty()) {
				// Check passenger route
				// False : Get off (Ride other train)
				if (PassengerRoute.Peek() == NextStation.Id) {
					PassengerRoute.Dequeue();
					Passenger[i]->SetPassengerPath(PassengerRoute);
				} else {
					Station->GetOffPassenger(Passenger[i]);
					Passenger.Add(i, nullptr);
					UpdatePassengerMesh();
					return;
				}
			}else {
				Station->GetOffPassenger(Passenger[i]);
				Passenger.Add(i, nullptr);
				UpdatePassengerMesh();
				return;
			}
		}
	}
	
	// Call when any passenger get off
	// Stop get off, Start get on
	GetWorld()->GetTimerManager().ClearTimer(GetOffHandle);
	GetWorld()->GetTimerManager().SetTimer(
		GetOnHandle,
		GetOnDelegate,
		1.0f,
		true,
		0.0f
	);

}

void ATrain::SetSubtrain(ASubtrain* T) {
	Cast<ASubtrainAiController>(T->GetController())->SetTargetTrain(this);
	Subtrains.Add(T);
}

