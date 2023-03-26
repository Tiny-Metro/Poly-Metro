// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainTemplate.h"
#include "Train/TrainManager.h"
#include "GameModes/GameModeBaseSeoul.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Lane/LaneManager.h"
#include <Engine/AssetManager.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ATrainTemplate::ATrainTemplate()
{
	//APlayerController::bEnableClickEvents = true;

	// Init train static mesh
	TrainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train Mesh"));

	//SetRootComponent(CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot"));

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TrainMovement = GetCharacterMovement();
	TrainMovement->MaxAcceleration = 50000.0f; // Default 2048
	TrainMovement->Mass = 1.0f; // Deafult 100
	TrainMovement->MaxWalkSpeed = 300.0f; // Default 600
	TrainMovement->bUseControllerDesiredRotation = true; // Default false

	TrainMaterial.AddUnique(
		ConstructorHelpers::FObjectFinder<UMaterial>(*TrainDefaultMaterialPath).Object
	);
	
	// Add dummy material (Material index start "1")
	PassengerMaterial.AddUnique(
		ConstructorHelpers::FObjectFinder<UMaterial>(*TrainDefaultMaterialPath).Object
	);

	auto PassengerScene = CreateDefaultSubobject<USceneComponent>("Passengers");
	PassengerScene->SetupAttachment(GetRootComponent());

	// Set passenger mesh
	for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		FName name = *FString::Printf(TEXT("Passenger %d"), i);
		auto tmp = CreateDefaultSubobject<UStaticMeshComponent>(name);
		tmp->SetupAttachment(PassengerScene);
		tmp->SetWorldRotation(PassengerMeshRotation);
		PassengerMeshComponent.Add(MoveTemp(tmp));
	}

	// Load meshes (Passenger)
	for (auto& i : PassengerMeshPath) {
		PassengerMesh.AddUnique(ConstructorHelpers::FObjectFinder<UStaticMesh>(*i).Object);
	}

	for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		Passenger.Add(i, nullptr);
	}

	// Bind click, release event
	OnClicked.AddDynamic(this, &ATrainTemplate::TrainOnClicked);
	OnReleased.AddDynamic(this, &ATrainTemplate::TrainOnReleased);

	//TrainMeshComponent->OnClicked.AddDynamic(this, &ATrainTemplate::TrainOnClicked);
	//TrainMeshComponent->OnReleased.AddDynamic(this, &ATrainTemplate::TrainOnReleased);
}

// Called when the game starts or when spawned
void ATrainTemplate::BeginPlay()
{
	Super::BeginPlay();

	LaneManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetLaneManager();
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	StationManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));

	InitTrainMaterial();
	InitPassengerMaterial();
	InitTrainMesh();

}

void ATrainTemplate::UpdatePassengerMesh() {
	// Read passenger array, clear and reorganize meshes
	for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		if (Passenger[i]) {
			PassengerMeshComponent[i]->SetStaticMesh(PassengerMesh[(int)Passenger[i]->GetDestination()]);
		} else {
			PassengerMeshComponent[i]->SetStaticMesh(nullptr);
		}
	}
}

AActor* ATrainTemplate::ConvertMousePositionToWorldLocation(FVector& WorldLocation) {
	FVector2D ScreenLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector WorldPosition, WorldDirection;
	FHitResult HitResult;
	LineTraceIgnoreActors.AddUnique(this);
	UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		ScreenLocation * UWidgetLayoutLibrary::GetViewportScale(GetWorld()),
		WorldPosition, WorldDirection);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), WorldPosition, (WorldDirection * 10000000.0f) + WorldPosition,
		ETraceTypeQuery::TraceTypeQuery1, false, LineTraceIgnoreActors, EDrawDebugTrace::Type::None,
		HitResult, true);
	WorldLocation = HitResult.Location;
	return HitResult.GetActor();
}

void ATrainTemplate::SetTrainMaterial(ALane* Lane) {
}

bool ATrainTemplate::IsPassengerSlotFull() {
	return false;
}

int32 ATrainTemplate::GetValidSeatCount() const {
	int32 ValidSeat = 0;
	for (int i = 0; i < CurrentPassengerSlot; i++) {
		if (!Passenger[i]) {
			ValidSeat++;
		}
	}
	return ValidSeat;
}

bool ATrainTemplate::AddPassenger(UPassenger* P) {
	for (int i = 0; i < Passenger.Num(); i++) {
		if (Passenger[i] == nullptr) {
			Passenger.Add(i, P);
			return true;
		}
	}
	return false;
}

void ATrainTemplate::DespawnTrain() {
	
}

void ATrainTemplate::DropPassenger() {
	auto CurrentStationPointer = StationManagerRef->GetStationByStationInfo(CurrentStation);
	auto NextStationPointer = StationManagerRef->GetStationByStationInfo(NextStation);

	if (IsValid(CurrentStationPointer)) {
		for (auto& i : Passenger) {
			if (i.Value != nullptr) {
				CurrentStationPointer->GetOffPassenger(i.Value);
				i.Value = nullptr;
				UpdatePassengerMesh();
			}
		}
	} else {

	}
}

FStationInfo ATrainTemplate::GetCurrentStation() const {
	return CurrentStation;
}

FStationInfo ATrainTemplate::GetNextStation() const {
	return NextStation;
}

void ATrainTemplate::TrainOnClicked(AActor* Target, FKey ButtonPressed) {
	TouchInput = true;
	TouchTime = 0.0f;
	OnPressedTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
}

void ATrainTemplate::TrainOnReleased(AActor* Target, FKey ButtonPressed) {
	IsActorDragged = false;
	TouchInput = false;
	TouchTime = 0.0f;
}

// Called every frame
void ATrainTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		TEXT("TrainTemplate::Tick")
	);*/
	TotalTravel += TrainMovement->MaxWalkSpeed * DeltaTime;

	if (TouchInput) {
		// Check train pressed time
		TouchTime += DeltaTime;
		if (TouchTime > LongClickInterval) {
			IsActorDragged = true;
		} else {
			IsActorDragged = false;
		}
	}
}

// Called to bind functionality to input
void ATrainTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATrainTemplate::InitTrainMaterial() {
	/*TrainMaterialPath = Cast<AGameModeBaseSeoul>(GetWorld()->GetAuthGameMode())->GetTrainMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(
		TrainMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ATrainTemplate::TrainMaterialDeferred)
	);*/

	auto tmp = Cast<AGameModeBaseSeoul>(GetWorld()->GetAuthGameMode())->GetTrainManager()->GetTrainMaterial();
	TrainMaterial.Append(tmp);
}

void ATrainTemplate::InitTrainMesh() {
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(
		TrainMeshPath,
		FStreamableDelegate::CreateUObject(this, &ATrainTemplate::TrainMeshDeferred)
	);
}

void ATrainTemplate::TrainMaterialDeferred() {
	for (auto& i : TrainMaterialPath) {
		//TAssetPtr<UMaterial> tmp(i);
		TrainMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

void ATrainTemplate::TrainMeshDeferred() {
	for (auto& i : TrainMeshPath) {
		TrainMesh.AddUnique(Cast<UStaticMesh>(i.ResolveObject()));
	}
}

void ATrainTemplate::InitPassengerMaterial() {
	auto tmp = Cast<AGameModeBaseSeoul>(GetWorld()->GetAuthGameMode())->GetTrainManager()->GetPassengerMaterial();
	PassengerMaterial.Append(tmp);
}

void ATrainTemplate::SetTrainId(int32 Id) {
	TrainId = Id;
}

int32 ATrainTemplate::GetTrainId() const {
	return TrainId;
}

void ATrainTemplate::SetServiceLaneId(int32 Id) {
	ServiceLaneId = Id;
}

int32 ATrainTemplate::GetServiceLaneId() const {
	return ServiceLaneId;
}

void ATrainTemplate::SetTrainDirection(TrainDirection Dir) {
	Direction = Dir;
}

TrainDirection ATrainTemplate::GetTrainDirection() const {
	return Direction;
}

void ATrainTemplate::Upgrade() {
	IsUpgrade = true;

	//TODO : Mesh change
}

void ATrainTemplate::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : TrainTemplate"));
}

FVector ATrainTemplate::GetNextTrainDestination(FVector CurLocation) {
	return FVector();
}

