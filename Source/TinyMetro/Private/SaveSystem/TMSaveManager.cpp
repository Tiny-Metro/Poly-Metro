// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/TMSaveManager.h"
#include "../../Public/Station/StationManager.h"
#include "../../Public/Station/Station.h"
#include "PlayerState/TinyMetroPlayerState.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATMSaveManager::ATMSaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATMSaveManager::BeginPlay()
{
	Super::BeginPlay();

	//stationmanager = Cast<AStationManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AStationManager::StaticClass()));
	stationmanager = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetStationManager();

	//TinyMetroPlayerState = GetWorld()->GetPlayerControllerIterator()->Get()->GetPlayerState<ATinyMetroPlayerState>();
	TinyMetroPlayerState = Cast<ATinyMetroPlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0));
	if (IsValid(TinyMetroPlayerState)) {
		UE_LOG(LogTemp, Log, TEXT("TMSaveManager::BeginPlay : PlayerState Valid"));
	} else {
		UE_LOG(LogTemp, Log, TEXT("TMSaveManager::BeginPlay : PlayerState Invalid"));
	}


	LoadWorldInfo();
	LoadStationManager();

	AutoSave();
	
}

// Called every frame
void ATMSaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void ATMSaveManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (!testing) {
		SaveWorldInfo();
		SaveStationManager();
	}
}

void ATMSaveManager::AutoSave() {

	GetWorld()->GetTimerManager().SetTimer(
		TimerAutoSave,
		FTimerDelegate::CreateLambda([&]() {
			AutoSaveCurrent += AutoSaveCount;
			if (AutoSaveCurrent >=AutoSaveRequire) {
		
				SaveWorldInfo();
				SaveStationManager();
		
				AutoSaveCurrent = 0.0f;
			}
		}),
		1.0f,
		true,
		1.0f
	);
}

void ATMSaveManager::SaveStationManager() {

	UStationSaveGame* StationSaveData;

	if (UGameplayStatics::DoesSaveGameExist("StationSave", 0)) {
		StationSaveData = Cast<UStationSaveGame>(UGameplayStatics::LoadGameFromSlot("StationSave", 0));
	}
	else {
		StationSaveData = Cast<UStationSaveGame>(UGameplayStatics::CreateSaveGameObject(UStationSaveGame::StaticClass()));
	}

	if (StationSaveData == NULL || stationmanager == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("stationmanager is null!"));
		return;
	}

	StationSaveData->StationSpawnCurrent = stationmanager->StationSpawnCurrent;
	StationSaveData->ManagerStationId = stationmanager->StationId;

	int32 size = stationmanager->Station.Num();

	//UE_LOG(LogTemp, Error, TEXT("SaveStationManager Station.num : %d"), stationmanager->Station.Num());

	StationSaveData->stations.Empty();

	for (int i = 0; i < size; i++)
	{
	
		AStation* temp = stationmanager->Station[i];

		
		int32 passengersSize = temp->Passenger.Num();

		FStationValuesStruct stationValue;

		stationValue.ComplainCurrent = temp->ComplainCurrent;
		stationValue.StationId = temp->StationId;
		stationValue.IsActive = temp->IsActive;
		stationValue.StationTypeValue = temp->StationTypeValue;
		stationValue.GridCellData = temp->CurrentGridCellData;
		//complain, stationid, isActive, stationtypevalue

		for (int j = 0; j < passengersSize; j++) {
			UPassenger* passenger = temp->Passenger[j];

			FPassengerValues passengerValue;

			passengerValue.Destination = passenger->Destination;

			stationValue.passengers.Add(passengerValue);
		}

		StationSaveData->stations.Add(stationValue);

		/*
		UE_LOG(LogTemp, Error, TEXT("i : %d"), i);
		UE_LOG(LogTemp, Error, TEXT("GridCellData : %d"), temp->GridCellData.Index);

		UE_LOG(LogTemp, Error, TEXT("stations.num : %d"), StationSaveData->stations.Num());*/
	}

	if (!UGameplayStatics::SaveGameToSlot(StationSaveData, "StationSave", 0))
	{
		UE_LOG(LogTemp, Error, TEXT("StationSaveGame Error!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("StationSaveGame Success!"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("SaveStationManager stations.num : %d"), StationSaveData->stations.Num());
}

void ATMSaveManager::LoadStationManager() {

	if (UGameplayStatics::DoesSaveGameExist("StationSave", 0))
	{
		UStationSaveGame* StationLoadData = Cast<UStationSaveGame>(UGameplayStatics::LoadGameFromSlot("StationSave", 0));

		// 액터 배치?

		if (stationmanager == nullptr) {
			return;
		}

		stationmanager->StationSpawnCurrent = StationLoadData->StationSpawnCurrent;
		stationmanager->StationId = StationLoadData->ManagerStationId;

		stationmanager->Station.Empty();

		int32 size = StationLoadData->stations.Num();

		//UE_LOG(LogTemp, Warning, TEXT("StationLoadData->station.num : %d"), StationLoadData->stations.Num());

		for (int i = 0; i < size; i++)
		{
			FStationValuesStruct stationValue = StationLoadData->stations[i];

			SpawnStations(stationValue );
		}

		//UE_LOG(LogTemp, Warning, TEXT("StationManagerLoading Success! Station.num : %d"), stationmanager->Station.Num());
	}

}

void ATMSaveManager::DeleteSaveFiles() {
	if (UGameplayStatics::DoesSaveGameExist("StationSave", 0)) {
		bool check =  UGameplayStatics::DeleteGameInSlot("StationSave", 0);
	}

	if (UGameplayStatics::DoesSaveGameExist("WorldInfoSave", 0)) {
		bool check = UGameplayStatics::DeleteGameInSlot("WorldInfoSave", 0);
	}

	testing = true;

	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void ATMSaveManager::SpawnStations(FStationValuesStruct StationValues) {
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Station/BP_Station.BP_Station'")));

	FGridCellData _GridCellData = StationValues.GridCellData;

	


	// Cast to BP
	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);

	// Check object validation
	if (!SpawnActor) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return;
	}

	// Check null
	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return;
	}

	// Spawn actor
	FActorSpawnParameters SpawnParams;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(_GridCellData.WorldLocation);
	SpawnParams.Owner = stationmanager;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//AStation* tmp = Cast<AStation>(stationmanager->GetWorld()->SpawnActor<AActor>(GeneratedBP->GeneratedClass, _GridCellData.WorldLocation, stationmanager->GetActorRotation(), SpawnParams));
	AStation* tmp = Cast<AStation>(stationmanager->GetWorld()->SpawnActorDeferred<AActor>(GeneratedBP->GeneratedClass, SpawnTransform));
	tmp->LoadStationValue(StationValues);
	tmp->SetPolicy(stationmanager->Policy);

	tmp->FinishSpawning(SpawnTransform);

	stationmanager->Station.Add(tmp);
	stationmanager->GridManager->SetGridStructure(
		_GridCellData.WorldCoordination.X,
		_GridCellData.WorldCoordination.Y,
		GridStructure::Station);
	stationmanager->GridManager->SetGridStation(
		_GridCellData.WorldCoordination.X,
		_GridCellData.WorldCoordination.Y,
		GridStationStructure::Station);

	//UE_LOG(LogTemp, Warning, TEXT("Station.Num : %d"), stationmanager->Station.Num());
}


void ATMSaveManager::SaveWorldInfo() {

	UWorldSaveGame* WorldInfoSaveData;

	if (UGameplayStatics::DoesSaveGameExist("WorldInfoSave", 0)) {
		WorldInfoSaveData = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot("WorldInfoSave", 0));
	}
	else {
		WorldInfoSaveData = Cast<UWorldSaveGame>(UGameplayStatics::CreateSaveGameObject(UWorldSaveGame::StaticClass()));
	}

	if (WorldInfoSaveData == NULL) {
		return;
	}

	WorldInfoSaveData->ElapseTimeSec = TinyMetroPlayerState->GetPlayTimeSec();

	if (!UGameplayStatics::SaveGameToSlot(WorldInfoSaveData, "WorldInfoSave", 0))
	{
		UE_LOG(LogTemp, Error, TEXT("StationSaveGame Error!"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("StationSaveGame Success!"));
	}

	float deltaseconds = WorldInfoSaveData->ElapseTimeSec;

	UE_LOG(LogTemp, Warning, TEXT("save ElapseTimeSec : %f"), deltaseconds);

}

void ATMSaveManager::LoadWorldInfo() {

	if (UGameplayStatics::DoesSaveGameExist("WorldInfoSave", 0))
	{
		UWorldSaveGame* WorldInfoLoadData = Cast<UWorldSaveGame>(UGameplayStatics::LoadGameFromSlot("WorldInfoSave", 0));

		TinyMetroPlayerState->SetPlayTimeSec(WorldInfoLoadData->ElapseTimeSec);

		UE_LOG(LogTemp, Warning, TEXT("WorldLoading Success!"));

		float deltaseconds = TinyMetroPlayerState->GetPlayTimeSec();

		UE_LOG(LogTemp, Warning, TEXT("load ElapseTimeSec : %f"), deltaseconds);

		//UE_LOG(LogTemp, Warning, TEXT("load GetDay : %d"), TinyMetroPlayerState->GetDay());

		//UE_LOG(LogTemp, Warning, TEXT("load GetDayTime : %f"), TinyMetroPlayerState->GetDayTime());

	}
}
