// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SpawnTrainTemplate.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "GridGenerator/GridManager.h"
#include "Station/StationManager.h"
#include "Train/TrainManager.h"
#include <UMG/Public/Blueprint/WidgetLayoutLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetSystemLibrary.h>


void USpawnTrainTemplate::NativeConstruct() {
	Super::NativeConstruct();
	UE_LOG(LogTemp, Log, TEXT("SpawnTrainTemplate::NativeConstruct : Call"));
	ATinyMetroGameModeBase* GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
	GridManagerRef = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));
	StationManagerRef = GameModeRef->GetStationManager();
	TrainManagerRef = GameModeRef->GetTrainManager();
	LaneManagerRef = GameModeRef->GetLaneManager();

}

AActor* USpawnTrainTemplate::ConvertMousePositionToWorldLocation(FVector& WorldLocation) {
	FVector2D ScreenLocation = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
	FVector WorldPosition, WorldDirection;
	FHitResult HitResult;
	//UE_LOG(LogTemp, Log, TEXT("Screen Location : %lf %lf"), ScreenLocation.X, ScreenLocation.Y);
	//UE_LOG(LogTemp, Log, TEXT("ViewportScale : %f"), UWidgetLayoutLibrary::GetViewportScale(GetWorld()));
	UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(GetWorld(), 0),
		ScreenLocation * UWidgetLayoutLibrary::GetViewportScale(GetWorld()),
		WorldPosition, WorldDirection);
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), WorldPosition, (WorldDirection * 10000000.0f) + WorldPosition,
		ETraceTypeQuery::TraceTypeQuery1, false, LineTraceIgnoreActors, EDrawDebugTrace::Type::None,
		HitResult, true);
	WorldLocation = HitResult.Location;
	WorldLocation.Z = 20.f;
	return HitResult.GetActor();
}
