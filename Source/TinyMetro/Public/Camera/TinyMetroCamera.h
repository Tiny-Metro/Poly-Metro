// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "TinyMetroCamera.generated.h"

class USpringArmComponent;
class UCameraComponent;

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
	
	//UFUNCTION()
	void InitViewport(class FViewport* Viewport, uint32 unused);

	UFUNCTION()
	void MouseWheel(float Axis);

	UFUNCTION()
	void MouseX(float Axis);
	UFUNCTION()
	void MouseY(float Axis);

	UFUNCTION()
	void CameraMoveX(int32 DirectionFlag, float DeltaTime);
	UFUNCTION()
	void CameraMoveY(int32 DirectionFlag, float DeltaTime);

	UFUNCTION()
	void SetCameraMoveEnable(bool Flag);

	UFUNCTION()
	void PinchZoom(float Value);

	//// Enhanced input
	//UFUNCTION()
	//void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Touch1Press();
	UFUNCTION()
	void Touch1Release();
	UFUNCTION()
	void Touch1DoubleClick();
	UFUNCTION()
	void Touch1Axis(float Axis);

	UFUNCTION()
	void Touch2Press();
	UFUNCTION()
	void Touch2Release();
	UFUNCTION()
	void Touch2Axis(float Axis);
	


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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	int32 ScreenX;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	int32 ScreenY;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
	FRotator CurrentRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	bool CameraMoveEnable = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveSpeedX = 0.05f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveSpeedY = 0.05f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMinX = -12000.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMaxX = 12000.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMinY = -15000.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMaxY = 15000.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinZoom = 3000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxZoom = 20000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomSpeed = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float CurrentZoom = 8000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomPinchPreTick = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	double Touch2StartDistance = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotation")
	float MinRotationAxisY = -90.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotation")
	float MaxRotationAxisY = -20.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotation")
	double CameraRotationSpeedX = 1.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotation")
	double CameraRotationSpeedY = 1.0;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//class UBoxComponent* BoxComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	USpringArmComponent* SpringArmComponenet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	bool Touch1Pressed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	bool Touch2Pressed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	float Touch1PressTime = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	bool IsRotationMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	bool IsMoveMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	FVector2D Touch1InitPosition;

	//// Enhanced input
	//UPROPERTY(EditAnywhere, Category = "Input");
	//class UInputMappingContext* InputContext;
	//UPROPERTY(EditAnywhere, Category = "Input");
	//class UInputAction* MoveAction;

};
