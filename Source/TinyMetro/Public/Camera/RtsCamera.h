#pragma once
#include "GameFramework/Pawn.h"
#include "RtsCamera.generated.h"

class UFloatingPawnMovement;
class USphereComponent;

/**
 * Default Pawn used with conjunction with RtsCameraController.
 */

UCLASS() 
class TINYMETRO_API ARtsCamera: public APawn
{
	GENERATED_UCLASS_BODY()

	void SetupPlayerInputComponent(UInputComponent* Inputelemental) override;
	void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category = "Sensitivity")
		float PitchChangeRate = 90.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
		float YawRate = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
		float PanRate = 50.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
		float ZoomRate = 10.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
		float ScreenEdgePanZonePercent = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Bounds")
		float MaxPitch = 335.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
		float MinPitch = 285.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
		float MaxZoom = 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
		float MinZoom = 300.0f;


	void PanForward(float magnitude);
	void PanRight(float magnitude);
	void ZoomIn(float magnitude);
	void ChangePitch(float magnitude);
	void Yaw(float magnitude);
	void EngageRotation();
	void DisengageRotation();
	void Orbit(float magnitude);
	void JumpTo(FVector NewLocation);
	bool rotationMode = false;


	FVector2D mouseScreenLocationGrab;
	UFloatingPawnMovement* MovementComponent;
	USphereComponent* CustomCollisionComponent;
};