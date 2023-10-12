// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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

	UFUNCTION()
	void ToggleCameraMoveEnable();
	UFUNCTION()
	void SetCameraMoveEnable(bool Flag);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//UFUNCTION()
	void InitViewport(class FViewport* Viewport, uint32 unused);

	// Used Windows
	UFUNCTION()
	void MouseWheel(float Axis);
	UFUNCTION()
	void CameraMoveX(float Axis);
	UFUNCTION()
	void CameraMoveY(float Axis);
	UFUNCTION()
	void CameraRotationZ(float Axis);
	UFUNCTION()
	void CameraRotationY(float Axis);

	// Used Mobile
	UFUNCTION()
	void Touch1Press();
	UFUNCTION()
	void Touch1Release();
	UFUNCTION()
	void Touch1Axis(float Axis);
	UFUNCTION()
	void Touch2Press();
	UFUNCTION()
	void Touch2Release();
	UFUNCTION()
	void Touch2Axis(float Axis);

	// Used Common
	UFUNCTION(BlueprintCallable)
	void ResetRotation();
	UFUNCTION(BlueprintCallable)
	void MoveCamera(FVector2D TargetLocation);

	// Save & Load
	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY()
	class ATinyMetroPlayerController* PlayerControllerRef;
	UPROPERTY()
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FVector2D MousePosition;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 ScreenX;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	int32 ScreenY;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Config")
	FRotator CurrentRotation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	bool IsAutoMovement = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	FVector2D TargetDistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	FVector2D TargetMoveLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraAutoMoveSeconds = 1.0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	bool CameraMoveEnable = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Move")
	bool UiOpened = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveSpeedX = 0.02f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveSpeedY = 0.02f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMinX = -12000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMaxX = 12000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMinY = -15000.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
	double CameraMoveBoundaryMaxY = 15000.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinZoom = 3000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxZoom = 20000.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomSpeed = 5.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Zoom")
	float CurrentZoom = 8000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	double Touch2StartDistance = 0.0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation")
	bool IsResetRotation = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	double CameraRestSeconds = 0.5;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rotation")
	double TargetResetRotation = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	double MinRotationAxisY = -90.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	double MaxRotationAxisY = -20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	double CameraRotationSpeedZ = 1.0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation")
	double CameraRotationSpeedY = 1.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	USpringArmComponent* SpringArmComponenet;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	bool Touch1Pressed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	bool Touch2Pressed = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	float Touch1PressTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float LongTouchInterval = 0.7f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	bool IsRotationMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	bool IsMoveMode = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Input")
	FVector2D Touch1InitPosition;
};
