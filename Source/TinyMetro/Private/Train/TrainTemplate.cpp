// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/TrainTemplate.h"
#include "Train/TrainManager.h"
#include "Train/TrainInfoWidget.h"
#include "Sound/SoundManager.h"
#include "GameModes/GameModeBaseSeoul.h"
#include "Camera/TinyMetroPlayerController.h"
#include "Camera/TinyMetroCamera.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "Lane/LaneManager.h"
#include "Lane/Lane.h"
#include "Station/PathQueue.h"
#include "Statistics/StatisticsManager.h"
#include "SaveSystem/TMSaveManager.h"
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
	TrainMovement->MaxAcceleration = 3000.0f; // Default 2048
	TrainMovement->Mass = 1.0f; // Deafult 100
	TrainMovement->MaxWalkSpeed = TRAIN_DEFAULT_SPEED; // Default 600
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

	/*for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		Passenger.Add(i, nullptr);
	}*/

	// Bind click, release event
	OnClicked.AddDynamic(this, &ATrainTemplate::TrainOnPressed);
	OnReleased.AddDynamic(this, &ATrainTemplate::TrainOnReleased);

	OnInputTouchBegin.AddDynamic(this, &ATrainTemplate::TrainTouchBegin);
	OnInputTouchEnd.AddDynamic(this, &ATrainTemplate::TrainTouchEnd);
}

// Called when the game starts or when spawned
void ATrainTemplate::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Log, TEXT("TrainTemplate::BeginPlay"));

	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	LaneManagerRef = GameModeRef->GetLaneManager();
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	StationManagerRef = GameModeRef->GetStationManager();
	PlayerStateRef = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	TrainManagerRef = GameModeRef->GetTrainManager();
	StatisticsManagerRef = GameModeRef->GetStatisticsManager();
	TimerRef = GameModeRef->GetTimer();
	SaveManagerRef = GameModeRef->GetSaveManager();
	SoundManagerRef = GameModeRef->GetSoundManager();

	PlayerControllerRef = Cast<ATinyMetroPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	InitTrainMaterial();
	InitPassengerMaterial();
	//InitTrainMesh();

	TimerRef->WeeklyTask.AddDynamic(this, &ATrainTemplate::WeeklyTask);

	SaveManagerRef->SaveTask.AddDynamic(this, &ATrainTemplate::Save);
}

void ATrainTemplate::UpdatePassengerMesh() {
	// Read passenger array, clear and reorganize meshes
	for (int i = 0; i < MaxPassengerSlotUpgrade; i++) {
		if (Passenger.Contains(i)) {
			PassengerMeshComponent[i]->SetStaticMesh(PassengerMesh[(int)Passenger[i].Destination]);
		} else {
			PassengerMeshComponent[i]->SetStaticMesh(nullptr);
		}
	}
}

AActor* ATrainTemplate::ConvertMousePositionToWorldLocation(FVector& WorldLocation) {
	FHitResult HitResult;
	FHitResult LineTraceResult;
	FVector2D ScreenLocation;
	//bool currentlyPressed;
	if (UGameplayStatics::GetPlatformName() == TEXT("Windows")) {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
		//ScreenLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	} else {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderFingerByChannel(ETouchIndex::Touch1, ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
		//UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetInputTouchState(ETouchIndex::Touch1, ScreenLocation.X, ScreenLocation.Y, currentlyPressed);
	}

	/*int32 ScreenX, ScreenY;
	PlayerControllerRef->GetViewportSize(ScreenX, ScreenY);
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Magenta, FString::Printf(TEXT("Screen X Y : %d %d"), ScreenX, ScreenY));
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Screen X Y : %f %f"), ScreenLocation.X, ScreenLocation.Y));*/

	FVector WorldPosition, WorldDirection;
	LineTraceIgnoreActors.AddUnique(this);
	/*UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		ScreenLocation * UWidgetLayoutLibrary::GetViewportScale(GetWorld()),
		WorldPosition, WorldDirection);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), WorldPosition, (WorldDirection * 1000000000.0f) + WorldPosition,
		ETraceTypeQuery::TraceTypeQuery1, false, LineTraceIgnoreActors, EDrawDebugTrace::Type::None,
		LineTraceResult, true);*/
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd,
		ETraceTypeQuery::TraceTypeQuery1, false, LineTraceIgnoreActors, EDrawDebugTrace::Type::None,
		LineTraceResult, true);
	WorldLocation = LineTraceResult.Location;
	WorldLocation.Z = 20.f;
	return LineTraceResult.GetActor();
}

void ATrainTemplate::SetTrainMaterial(ALane* Lane) {
	if (IsValid(Lane)) {
		if (!TrainMaterial.IsValidIndex(Lane->GetLaneId())) {
			InitTrainMaterial();
		}
		if (!PassengerMaterial.IsValidIndex(Lane->GetLaneId())) {
			InitPassengerMaterial();
		}
		//UE_LOG(LogTemp, Log, TEXT("TrainTemplate::SetTrainMaterial %d %d"), TrainMaterial.Num(), PassengerMaterial.Num());
		TrainMeshComponent->SetMaterial(0, TrainMaterial[Lane->GetLaneId()]);
		//TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
		for (auto& i : PassengerMeshComponent) {
			i->SetMaterial(0, PassengerMaterial[Lane->GetLaneId()]);
		}
	} else {
		TrainMeshComponent->SetMaterial(0, TrainMaterial[0]);
	}
}

bool ATrainTemplate::IsPassengerSlotFull() {
	return false;
}

int32 ATrainTemplate::GetValidSeatCount() const {
	/*int32 ValidSeat = 0;
	for (int i = 0; i < CurrentPassengerSlot; i++) {
		if (!Passenger[i]) {
			ValidSeat++;
		}
	}*/
	return CurrentPassengerSlot - Passenger.Num();
}

bool ATrainTemplate::AddPassenger(FPassenger P) {
	for (int i = 0; i < CurrentPassengerSlot; i++) {
		if (!Passenger.Contains(i)) {
			Passenger.Add(i, P);

			TrainInfo.TotalBoardPassenger++;
			TrainInfo.WeeklyBoardPassenger++;

			// TODO : Transfer passenger
			{
				FScopeLock Lock(StatisticsManagerRef->GetDefaultStatisticsKey().Pin().Get());
				StatisticsManagerRef->DefaultStatistics.TotalArrivePassenger++;
				StatisticsManagerRef->DefaultStatistics.WeeklyArrivePassenger++;
			}
			UpdatePassengerMesh();
			return true;
		}
	}
	return false;
}

void ATrainTemplate::UpdatePassengerSlot() {
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

void ATrainTemplate::UpdateTrainMesh() {
}

bool ATrainTemplate::CanUpgrade() const {
	return false;
}

int32 ATrainTemplate::GetTotalBoardPassenger() const {
	return TrainInfo.TotalBoardPassenger;
}

int32 ATrainTemplate::GetWeeklyBoardPassenger() const {
	return TrainInfo.WeeklyBoardPassenger;
}

void ATrainTemplate::ServiceStart(FVector StartLocation, ALane* Lane, AStation* D) {
	//Destination = D;
	StartLocation.Z = 20.f;
	this->SetActorLocation(StartLocation);
	if (!IsValid(StatisticsManagerRef)) StatisticsManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStatisticsManager();

	TrainManagerRef->AddTrain(this);
	TrainZAxis = this->GetActorLocation().Z;
	if (IsActorFirstSpawn) {
		TrainManagerRef->TrainTutorialTask.Broadcast(GetActorLocation());
		if (this->GetTrainInfo().Type == TrainType::Train) {
			StatisticsManagerRef->ShopStatistics.TrainStatistics.TotalPlacementCount++;
		} else {
			StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalPlacementCount++;
		}
		IsActorFirstSpawn = false;
	} else {
		if (this->GetTrainInfo().Type == TrainType::Train) {
			StatisticsManagerRef->ShopStatistics.TrainStatistics.TotalShiftCount++;
		} else {
			StatisticsManagerRef->ShopStatistics.SubtrainStatistics.TotalShiftCount++;
		}
	}
	IsActorSpawnByWidget = false;

	LaneRef = Lane;
	TrainInfo.ServiceLaneId = Lane->GetLaneId();
	TrainInfo.ShiftCount++;
	UpdateTrainMesh();
}

void ATrainTemplate::DespawnTrain() {
	DropPassenger();
	TrainManagerRef->RemoveTrain(this);
	this->Destroy();
}

void ATrainTemplate::DropPassenger() {
	auto CurrentStationPointer = StationManagerRef->GetStationByStationInfo(CurrentStation);
	auto NextStationPointer = StationManagerRef->GetStationByStationInfo(NextStation);

	if (IsValid(CurrentStationPointer)) {
		for (auto& i : Passenger) {
			if (i.Value.Destination == StationType::None) continue;
			CurrentStationPointer->GetOffPassenger(i.Value, this);
			Passenger.Remove(i.Key);
			UpdatePassengerMesh();
		}
	}
}

void ATrainTemplate::GetOffPassenger(AStation* Station, bool& Success) {
	Success = false;
	for (int i = 0; i < CurrentPassengerSlot; i++) {
		if (Passenger.Contains(i)) {
			// Update passenger route
			auto PassengerRoute = StationManagerRef->GetShortestPath(
				CurrentStation.Id,
				Passenger[i].Destination
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
					//Passenger[i]->SetPassengerPath(PassengerRoute);
				} else {
					Station->GetOffPassenger(Passenger[i], this);
					//Passenger.Add(i, nullptr);
					Passenger.Remove(i);
					//Passenger.Remove(i);
					UpdatePassengerMesh();
					Success = true;
					return;
				}
			} else {
				Station->GetOffPassenger(Passenger[i], this);
				//Passenger.Add(i, nullptr);
				Passenger.Remove(i);
				UpdatePassengerMesh();
				Success = true;
				return;
			}
		}
	}
}

FStationInfo ATrainTemplate::GetCurrentStation() const {
	return CurrentStation;
}

void ATrainTemplate::SetCurrentStation(FStationInfo Info) {
	CurrentStation = Info;
}

FStationInfo ATrainTemplate::GetNextStation() const {
	return NextStation;
}

void ATrainTemplate::SetNextStation(FStationInfo Info) {
	NextStation = Info;
}

void ATrainTemplate::TrainOnPressed(AActor* Target, FKey ButtonPressed) {
	OnPressedLogic();
}

void ATrainTemplate::TrainOnReleased(AActor* Target, FKey ButtonPressed) {
	OnReleasedLogic();
}

int32 ATrainTemplate::GetShiftCount() const {
	return TrainInfo.ShiftCount;
}

void ATrainTemplate::TrainTouchBegin(ETouchIndex::Type FingerIndex, AActor* TouchedActor) {
	OnPressedLogic();
}

void ATrainTemplate::TrainTouchEnd(ETouchIndex::Type FingerIndex, AActor* TouchedActor) {
	OnReleasedLogic();
}

void ATrainTemplate::OnPressedLogic() {
	UE_LOG(LogTemp, Log, TEXT("TrainTemplate::OnClickLogic"));
	TouchInput = true;
	TouchTime = 0.0f;
	TrainManagerRef->ClickedTrain = this;
	if (!IsValid(CameraRef)) CameraRef = Cast<ATinyMetroCamera>(PlayerStateRef->GetPawn());
	CameraRef->SetCameraMoveEnable(false);
	OnPressedTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
}

void ATrainTemplate::OnReleasedLogic() {
	UE_LOG(LogTemp, Log, TEXT("TrainTemplate::OnReleasedLogic"));
	/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
		TEXT("TrainTemplate::OnReleased")
	);*/
	if (TouchTime < LongClickInterval) {
		IsSingleClick = true;
		if (!IsValid(TrainInfoWidget)) SetTrainInfoWidget(TrainManagerRef->GetTrainInfoWidget());
		TrainInfoWidget->ShowWidget(this);
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue,
			TEXT("TrainTemplate::OnClick")
		);*/
	}
	if (!IsValid(CameraRef)) CameraRef = Cast<ATinyMetroCamera>(PlayerStateRef->GetPawn());
	CameraRef->SetCameraMoveEnable(true);
	IsActorDragged = false;
	TouchInput = false;
	TouchTime = 0.0f;
}


void ATrainTemplate::SetDespawnNextStation() {
	DeferredDespawn = true;
}

FTrainInfo ATrainTemplate::GetTrainInfo() {
	return TrainInfo;
}

void ATrainTemplate::SetTrainInfoWidget(UTrainInfoWidget* Widget) {
	TrainInfoWidget = Widget;
}

// Broadcast by TimerRef
void ATrainTemplate::WeeklyTask() {
	UE_LOG(LogTemp, Log, TEXT("TrainTemplate::WeeklyTask"));
	TrainInfo.WeeklyBoardPassenger = 0;
}

void ATrainTemplate::Save() {
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(TrainManagerRef)) TrainManagerRef = GameModeRef->GetTrainManager();
	if (!IsValid(LaneManagerRef)) LaneManagerRef = GameModeRef->GetLaneManager();
	if (!IsValid(StationManagerRef)) StationManagerRef = GameModeRef->GetStationManager();
}

bool ATrainTemplate::Load() {
	if (!IsValid(GameModeRef)) GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (!IsValid(SaveManagerRef)) SaveManagerRef = GameModeRef->GetSaveManager();
	if (!IsValid(TrainManagerRef)) TrainManagerRef = GameModeRef->GetTrainManager();
	if (!IsValid(LaneManagerRef)) LaneManagerRef = GameModeRef->GetLaneManager();
	if (!IsValid(StationManagerRef)) StationManagerRef = GameModeRef->GetStationManager();
	IsLoaded = true;
	IsActorFirstSpawn = false;
	return false;
}

void ATrainTemplate::FinishLoad() {
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

	//FHitResult HitResult;
	//FVector touchPosition;
	/*if (UGameplayStatics::GetPlatformName() == TEXT("Windows")) {
		PlayerControllerRef->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
	} else {
		PlayerControllerRef->GetHitResultUnderFingerByChannel(ETouchIndex::Touch1, ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
	}*/
	//ConvertMousePositionToWorldLocation(touchPosition);
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Hit : %f %f %f"), touchPosition.X, touchPosition.Y, touchPosition.Z));


	if (IsActorDragged) {
		DropPassenger();

		//MouseToWorldActor = ConvertMousePositionToWorldLocation(MouseToWorldLocation);
		MouseToWorldActor = PlayerControllerRef->ConvertMousePositionToWorldLocation(MouseToWorldLocation, LineTraceIgnoreActors);
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, FString::Printf(TEXT("Train:: %f %f %f"), MouseToWorldLocation.X, MouseToWorldLocation.Y, MouseToWorldLocation.Z));
		/*GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
			FString::Printf(TEXT("Train::Tick - %lf, %lf"), MouseToWorldLocation.X, MouseToWorldLocation.Y));*/
		LaneRef = Cast<ALane>(MouseToWorldActor);
		SetTrainMaterial(LaneRef);
		if (IsValid(MouseToWorldActor)) {
			if (MouseToWorldActor->IsA(AStation::StaticClass()) ||
				MouseToWorldActor->IsA(ATrainTemplate::StaticClass())) {
				LineTraceIgnoreActors.AddUnique(MouseToWorldActor);
			}
		}

	} else {
		auto locationTmp = GetActorLocation();
		if (locationTmp.Z != TrainZAxis) {
			locationTmp.Z = TrainZAxis;
			SetActorLocation(locationTmp);
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
	for (int i = 1; i < TrainMaterial.Num() - 1; i++) {
		TrainMaterial.RemoveAt(i);
	}
	TrainMaterial.Append(tmp);
}

//void ATrainTemplate::InitTrainMesh() {
//	auto& AssetLoader = UAssetManager::GetStreamableManager();
//	AssetLoader.RequestAsyncLoad(
//		TrainMeshPath,
//		FStreamableDelegate::CreateUObject(this, &ATrainTemplate::TrainMeshDeferred)
//	);
//}

void ATrainTemplate::TrainMaterialDeferred() {
	for (auto& i : TrainMaterialPath) {
		//TAssetPtr<UMaterial> tmp(i);
		TrainMaterial.AddUnique(Cast<UMaterial>(i.ResolveObject()));
	}
}

//void ATrainTemplate::TrainMeshDeferred() {
//	for (auto& i : TrainMeshPath) {
//		TrainMesh.AddUnique(Cast<UStaticMesh>(i.ResolveObject()));
//	}
//}

void ATrainTemplate::InitPassengerMaterial() {
	auto tmp = Cast<AGameModeBaseSeoul>(GetWorld()->GetAuthGameMode())->GetTrainManager()->GetPassengerMaterial();
	for (int i = 1; i < PassengerMaterial.Num() - 1; i++) {
		PassengerMaterial.RemoveAt(i);
	}
	PassengerMaterial.Append(tmp);
}

void ATrainTemplate::SetTrainSpeed(float Speed) {
	TrainMovement->MaxWalkSpeed = Speed;
}

void ATrainTemplate::SetTrainId(int32 Id) {
	TrainInfo.Id = Id;
}

int32 ATrainTemplate::GetTrainId() const {
	return TrainInfo.Id;
}

void ATrainTemplate::SetServiceLaneId(int32 Id) {
	TrainInfo.ServiceLaneId = Id;
}

int32 ATrainTemplate::GetServiceLaneId() const {
	return TrainInfo.ServiceLaneId;
}

void ATrainTemplate::SetTrainDirection(TrainDirection Dir) {
	Direction = Dir;
}

TrainDirection ATrainTemplate::GetTrainDirection() const {
	return Direction;
}

void ATrainTemplate::Upgrade() {
	TrainInfo.IsUpgrade = true;
	CurrentPassengerSlot = MaxPassengerSlotUpgrade;
	if (!IsLoaded) {
		SoundManagerRef->PlaySound(TinyMetroEffectSound::TrainUpgrade);
	}
	SetTrainSpeed(TRAIN_UPGRADE_SPEED);
	UpdateTrainMesh();
}

bool ATrainTemplate::GetIsUpgrade() const {
	return TrainInfo.IsUpgrade;
}

void ATrainTemplate::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : TrainTemplate"));
}

FVector ATrainTemplate::GetNextTrainDestination(FVector CurLocation) {
	return FVector();
}

