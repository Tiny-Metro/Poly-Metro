// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RtsController.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ARtsController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AutoCameraZoomIn(FVector Location);
	virtual void AutoCameraZoomIn_Implementation(FVector Location);
		
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AutoCameraZoomOut(FVector Location);
	virtual void AutoCameraZoomOut_Implementation(FVector Location);

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GameOverZoomIn(FVector Location);
	virtual void GameOverZoomIn_Implementation(FVector Location);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraScrollSpeed = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraScrollEdgeThreshhold = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraZoomSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraRotationSpeed = 200.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameoverZoomSpeed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayZoom = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float XLeftLimit = -500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float XRightLimit = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YDownLimit = 500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float YUpLimit = -500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayZoomOut = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator DefaultRotation = FRotator(0.0f, 0.0f, 0.0f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GameOverZoomInSpeed = 30.0f;
};
