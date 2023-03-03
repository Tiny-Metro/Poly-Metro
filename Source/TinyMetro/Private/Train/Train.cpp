// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"
#include "Train/SubtrainAiController.h"
#include "Station/Station.h"
#include "Lane/Lane.h"
#include "Lane/LaneManager.h"
#include "Components/BoxComponent.h"
#include "GameModes/GameModeBaseSeoul.h"
#include <GameFramework/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/AssetManager.h>

void ATrain::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Train"));
}

ATrain::ATrain() {
	ConstructorHelpers::FObjectFinder<UStaticMesh> LoadTrainMesh(
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_Train.SM_Train'")
	);
	TrainMesh.AddUnique(LoadTrainMesh.Object);
	/*static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(
		TEXT("Material'/Game/Resource/Material/Lane/M_Lane_8.M_Lane_8'")
	);
	TrainMaterial.AddUnique(DefaultMaterial.Object->GetMaterial());*/
	

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	OverlapVolume->InitBoxExtent(FVector(10,20,30));
	//OverlapVolume->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ATrain::OnOverlapBegin, FName("OnOverlapBegin"));
	OverlapVolume->OnComponentBeginOverlap.AddDynamic(this, &ATrain::OnOverlapBegin);
	//OverlapVolume->OnComponentEndOverlap.__Internal_AddDynamic(this, &ATrain::OnOverlapEnd, FName("OnOverlapEnd"));
	OverlapVolume->OnComponentEndOverlap.AddDynamic(this, &ATrain::OnOverlapEnd);
	OverlapVolume->SetupAttachment(RootComponent);

	TrainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train Mesh"));
	TrainMeshComponent->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(LoadTrainMesh.Object);
	//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	//TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);

	//TrainMaterial.Add(TrainMeshComponent->GetStaticMesh()->GetMaterial(0)->GetMaterial());

	for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		PassengerMeshComponent[i]->SetRelativeLocation(PassengerMeshPosition[i]);
	}
}

void ATrain::BeginPlay() {
	Super::BeginPlay();
	TotalTravel = 0.0f;
	AiControllerRef = Cast<ATrainAiController>(GetController());

	//UE_LOG(LogTemp, Log, TEXT("TUM : %s"), *TrainUpgradeMesh.ToString());

	
	
	// Material change test code
	/*GetWorld()->GetTimerManager().SetTimer(TestTimer, FTimerDelegate::CreateLambda([&]() {
				int tmp = FMath::RandRange(0, TrainMaterial.Num() - 1);
				TrainMeshComponent->SetMaterial(0, TrainMaterial[tmp]);
				UE_LOG(LogTemp, Log, TEXT("Material Change : %d"), tmp);
			}
		),
		1.0f,
		true,
		1.0f
	);*/

	/*if (IsValid(ParentAiControllerRef)) { // True
		UE_LOG(LogTemp, Log, TEXT("Success"));
	}*/

}

void ATrain::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AStation::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Overlap"));
		
		// Movement stop, release
		TrainMovement->SetActive(false);
		/*FLatentActionInfo f;
		f.CallbackTarget = this;
		f.ExecutionFunction = FName("ActiveMoveTest");
		f.Linkage = 1;
		f.UUID = 0;
		UKismetSystemLibrary::Delay(GetWorld(), 5.0f, f);*/
		
		// Ride passenger
		//RidePassengerTest();

		PassengerIndex = 0;

		/* Train::GetOffPassenger
		*/
		// AdjList[Index] : FAdjArrayItem
		// AdjList[Index].AdjItem

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
		
		GetWorld()->GetTimerManager().SetTimer(
			GetOffHandle,
			GetOffDelegate,
			1.0f,
			true,
			0.0f
		);


		//AiControllerRef->StopMovement();
	}
	
}

void ATrain::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor->IsA(AStation::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("End Overlap"));
	}
}



//void ATrain::Tick(float DeltaTime) { }

FVector ATrain::GetNextTrainDestination(FVector CurLocation) {
	UE_LOG(LogTemp, Log, TEXT("Train::GetNextTrainDestination"));
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
		TrainMeshComponent->GetStaticMesh()->SetMaterial(0, TrainMaterial[Lane->GetLaneId()]);
	} else {
		TrainMeshComponent->GetStaticMesh()->SetMaterial(0, TrainMaterial[0]);
	}
}

void ATrain::Upgrade() {
	// Change mesh
	TrainMeshComponent->SetStaticMesh(TrainMesh[0]);
	// Set flag
	IsUpgrade = true;
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
	SetServiceLaneId(Lane->GetLaneId());
	SetTrainDirection(Lane->SetDirectionInit(
		Destination,
		GridManagerRef->GetGridCellDataByCoord(StartLocation, tmp).WorldCoordination
	));
	AiControllerRef->SetTrainDestination(GetNextTrainDestination(StartLocation));
	AiControllerRef->Patrol();
}

void ATrain::ActiveMoveTest() {
	TrainMovement->SetActive(true);
}

void ATrain::GetOnPassenger(AStation* Station) {
	if (!IsPassengerSlotFull()) {
		auto RidePassenger = Station->GetOnPassenger(PassengerIndex++);

		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.0f,
				FColor::Magenta,
				FString::Printf(TEXT("GetOnPassenger::Call")));*/

		if (RidePassenger.Key != nullptr) {
			// Log
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(
					-1,
					1.0f,
					FColor::Magenta,
					FString::Printf(TEXT("Ride %d"), PassengerIndex));*/
			PassengerIndex--;
			if (!AddPassenger(RidePassenger.Key)) { // Train is full
				// Passenger get on subtrain
				for (auto& i : Subtrains) {
					if (i->AddPassenger(RidePassenger.Key)) {
						break;
					}
				}
			} 

			//Passenger.Add(MoveTemp(RidePassenger.Key));
			TotalPassenger++;
			UpdatePassengerMesh();
		}

		// Index invalid
		if (!RidePassenger.Value) {
			/*if (GEngine)
				GEngine->AddOnScreenDebugMessage(
					-1,
					1.0f,
					FColor::Black,
					FString::Printf(TEXT("Ride Finish")));*/
			GetWorld()->GetTimerManager().ClearTimer(GetOnHandle);
			TrainMovement->SetActive(true);
		}
	} else {
		GetWorld()->GetTimerManager().ClearTimer(GetOnHandle);
		TrainMovement->SetActive(true);
	}


	//RidePassenger = Station->GetOnPassenger(Index++);




	//if (!TrainMovement->IsActive()) {
	//	// Log
	//	if (GEngine)
	//		GEngine->AddOnScreenDebugMessage(
	//			-1,
	//			1.0f,
	//			FColor::Magenta,
	//			FString::Printf(TEXT("Ride %d"), RideCount++));
	//	//UKismetSystemLibrary::Delay(GetWorld(), 1.0f, RideAction);

	//	/*auto RidePassenger = Station->GetOnPassenger(Index);
	//	if (RidePassenger.Value) {
	//		Passenger.Add(MoveTemp(RidePassenger.Key));
	//	}*/

	//} else {
	//	GetWorld()->GetTimerManager().ClearTimer(RideHandle);
	//}
}

void ATrain::GetOffPassenger(AStation* Station) {
	for (int i = 0; i < CurrentPassengerSlot; i++) {
		if (Passenger[i]) {
			// Check passenger route
			if (true) {
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

