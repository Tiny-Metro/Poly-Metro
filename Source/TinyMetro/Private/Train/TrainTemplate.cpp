// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainTemplate.h"
#include "GameModes/GameModeBaseSeoul.h"
#include "Lane/LaneManager.h"
#include <Engine/AssetManager.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ATrainTemplate::ATrainTemplate()
{
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

	auto PassengerScene = CreateDefaultSubobject<USceneComponent>("Passengers");
	PassengerScene->SetupAttachment(RootComponent);

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
}

// Called when the game starts or when spawned
void ATrainTemplate::BeginPlay()
{
	Super::BeginPlay();

	LaneManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetLaneManager();
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	InitTrainMaterial();
	InitTrainMesh();



	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		TEXT("Train Spawn"));
	
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

//FVector ATrainTemplate::ConvertMousePositionToWorldLocation_Implementation() {
//	return FVector();
//}

FVector ATrainTemplate::ConvertMousePositionToWorldLocation() {
	FVector2D ScreenLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector WorldPosition, WorldDirection;
	FHitResult HitResult;
	TArray<AActor*> tmp;
	tmp.Add(this);
	UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		ScreenLocation * UWidgetLayoutLibrary::GetViewportScale(GetWorld()),
		WorldPosition, WorldDirection);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), WorldPosition, (WorldDirection * 10000.0f) + WorldPosition,
		ETraceTypeQuery::TraceTypeQuery1, false, tmp, EDrawDebugTrace::Type::None,
		HitResult, true);

	return HitResult.Location;
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



// Called every frame
void ATrainTemplate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TotalTravel += TrainMovement->MaxWalkSpeed * DeltaTime;
}

// Called to bind functionality to input
void ATrainTemplate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATrainTemplate::InitTrainMaterial() {
	TrainMaterialPath = Cast<AGameModeBaseSeoul>(GetWorld()->GetAuthGameMode())->GetTrainMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(
		TrainMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ATrainTemplate::TrainMaterialDeferred)
	);
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

