// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TinyMetroCamera.generated.h"

UCLASS()
class TINYMETRO_API ATinyMetroCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATinyMetroCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MouseWheel(float Axis);

	UFUNCTION()
	void MouseX(float Axis);
	UFUNCTION()
	void MouseY(float Axis);

	UFUNCTION()
	void CameraRotationOn();
	UFUNCTION()
	void CameraRotationOff();

	UFUNCTION()
	void CameraMoveX(int32 DirectionFlag, float DeltaTime);
	UFUNCTION()
	void CameraMoveY(int32 DirectionFlag, float DeltaTime);

	UFUNCTION()
	void SetCameraMoveEnable(bool Flag);

	UFUNCTION()
	void PinchZoom(float Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY()
	APlayerController* PlayerControllerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MousePosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	int32 ScreenX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	int32 ScreenY;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	bool CameraMoveEnable = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	float CameraMoveSpeedX = 20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	float CameraMoveSpeedY = 20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	float CameraMoveBoundary = 0.2f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Location")
	FVector InitLocation = FVector(0, 3500.0f, 6000.f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinZoom = 300.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxZoom = 3000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomSpeed = 20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomScale = 2000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotation")
	bool IsCameraRotation = false;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponenet;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class UCameraComponent* CameraComponent;
};
