// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/TinyMetroPlayerController.h"
#include <Kismet/GameplayStatics.h>

ATinyMetroPlayerController::ATinyMetroPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	bShowMouseCursor = true;
}

AActor* ATinyMetroPlayerController::ConvertMousePositionToWorldLocation(FVector& WorldLocation, const TArray<AActor*>& TraceIgnoreActor) {
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

	FVector WorldPosition, WorldDirection;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), HitResult.TraceStart, HitResult.TraceEnd,
		ETraceTypeQuery::TraceTypeQuery1, false, TraceIgnoreActor, EDrawDebugTrace::Type::None,
		LineTraceResult, true);
	WorldLocation = LineTraceResult.Location;
	WorldLocation.Z = 20.f;
	return LineTraceResult.GetActor();
}

