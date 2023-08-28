#pragma once
#include "GameFramework/Pawn.h"
#include "RtsCamera.generated.h"

class UFloatingPawnMovement;
class USphereComponent;

UCLASS()
class TINYMETRO_API ARtsCamera : public APawn
{
	GENERATED_UCLASS_BODY()

	void SetupPlayerInputComponent(UInputComponent* Inputelemental) override;
	void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float PitchChangeRate = 9000.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float YawRate = 15000.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float PanRate = 10000.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float ZoomRate = 1000000000.0f;
	UPROPERTY(EditAnywhere, Category = "Sensitivity")
	float ScreenEdgePanZonePercent = 15.0f;

	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MaxPitch = 335.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MinPitch = 285.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MaxZoom = 30000.0f;
	UPROPERTY(EditAnywhere, Category = "Bounds")
	float MinZoom = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Camera")
	FRotator InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Camera")
	float RotationSpeed = 1.0f;  // 회전 속도 조절

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Camera")
	FVector initLocation = FVector(0, 0, 6000.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Camera")
	FRotator initRotation = FRotator(0.f, -90.f, 0.f);

	UFUNCTION(BlueprintCallable, Category = "Camera")
	void JumpTo(FVector NewLocation);

	void PanForward();
	void PanRight();
	void PanBackward();
	void PanLeft();
	void ZoomIn(float magnitude);
	void ChangePitch(float magnitude);
	void Yaw(float magnitude);
	void EngageRotation();
	void DisengageRotation();
	void Orbit(float magnitude);
	bool rotationMode = false;
	
	FVector2D mouseLocation;
	FVector2D mouseScreenLocationGrab;

	UPROPERTY(BlueprintReadOnly)
	APlayerController* playerController;

	UPROPERTY(BlueprintReadOnly)
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(BlueprintReadOnly)
	USphereComponent* CustomCollisionComponent;

	FVector2D DeltaMouse;
	FRotator NewRotation;
	int32 screenWidth, screenHeight;
};