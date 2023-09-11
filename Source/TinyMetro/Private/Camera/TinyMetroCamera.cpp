// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/TinyMetroCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "UnrealClient.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ATinyMetroCamera::ATinyMetroCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	////BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	////BoxComponent->SetupAttachment(RootComponent);

	SpringArmComponenet = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponenet->SetWorldRotation(FRotator(-60.0f, 0.0f, 0.0f).Quaternion());
	SpringArmComponenet->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponenet);
	
}

// Called when the game starts or when spawned
void ATinyMetroCamera::BeginPlay()
{
	Super::BeginPlay();
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Pawn::BeginPlay"));
	if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
	PlayerControllerRef->GetViewportSize(ScreenX, ScreenY);

	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &ATinyMetroCamera::InitViewport);
	//RootComponent->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f).Quaternion());

	//SpringArmComponenet->SetUsingAbsoluteRotation(true);
	//SpringArmComponenet->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	SpringArmComponenet->TargetArmLength = CurrentZoom;
}

void ATinyMetroCamera::InitViewport(FViewport* Viewport, uint32 unused) {
	if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
	PlayerControllerRef->GetViewportSize(ScreenX, ScreenY);
}

void ATinyMetroCamera::MouseWheel(float Axis) {
	float currentArmLenght = SpringArmComponenet->TargetArmLength;
	if (Axis > 0 && currentArmLenght >= MinZoom) {
		// Zoom in
		SpringArmComponenet->TargetArmLength += -Axis * ZoomSpeed;
		if (SpringArmComponenet->TargetArmLength < MinZoom) {
			SpringArmComponenet->TargetArmLength = MinZoom;
		}
	} else if (Axis < 0 && currentArmLenght <= MaxZoom) {
		// Zoom out
		SpringArmComponenet->TargetArmLength += -Axis * ZoomSpeed;
		if (SpringArmComponenet->TargetArmLength > MaxZoom) {
			SpringArmComponenet->TargetArmLength = MaxZoom;
		}
	}
}

void ATinyMetroCamera::MouseX(float Axis) {
	if(Axis != 0.0f) UE_LOG(LogTemp, Log, TEXT("MouseX : %f"), Axis);
	if (IsRotationMode) {
		//AddActorWorldRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
		//CameraComponent->AddLocalRotation(FRotator(0.0f, Axis, 0.0f).Quaternion());
		
		SpringArmComponenet->AddLocalRotation(FRotator(0.0f, Axis, 0.0f).Quaternion());
	}
}

void ATinyMetroCamera::MouseY(float Axis) {
	if (Axis != 0.0f) UE_LOG(LogTemp, Log, TEXT("MouseY : %f"), Axis);
	if (IsRotationMode) {
		//SpringArmComponenet->AddLocalRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
		CameraComponent->AddLocalRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
		//CameraComponent->AddLocalRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
	}
}

void ATinyMetroCamera::CameraMoveX(int32 DirectionFlag, float DeltaTime) {
	// DirectionFlag > 0 : Move Right
	// DirectionFlag < 0 : Move Left
	if (CameraMoveEnable) {
		auto currentLocation = GetActorLocation();
		currentLocation.X += DirectionFlag * CameraMoveSpeedX * DeltaTime;
		SetActorLocation(currentLocation);
	}
}

void ATinyMetroCamera::CameraMoveY(int32 DirectionFlag, float DeltaTime) {
	// DirectionFlag > 0 : Move Down
	// DirectionFlag < 0 : Move Up
	if (CameraMoveEnable) {
		auto currentLocation = GetActorLocation();
		currentLocation.Y += DirectionFlag * CameraMoveSpeedX * DeltaTime;
		SetActorLocation(currentLocation);
	}
}

void ATinyMetroCamera::SetCameraMoveEnable(bool Flag) {
	CameraMoveEnable = Flag;
}

void ATinyMetroCamera::PinchZoom(float Value) {
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString::Printf(TEXT("PinchZoom : %f"), Value));
	SpringArmComponenet->TargetArmLength += Value * ZoomSpeed;
	if (SpringArmComponenet->TargetArmLength > MaxZoom) {
		SpringArmComponenet->TargetArmLength = MaxZoom;
	}
	if (SpringArmComponenet->TargetArmLength < MinZoom) {
		SpringArmComponenet->TargetArmLength = MinZoom;
	}
	//if (Value != 0.0f) {
	//	if (ZoomPinchPreTick > Value) {
	//		// Zoom out
	//		SpringArmComponenet->TargetArmLength += ZoomSpeed;
	//		if (SpringArmComponenet->TargetArmLength > MaxZoom) {
	//			SpringArmComponenet->TargetArmLength = MaxZoom;
	//		}
	//	} else {
	//		// Zoom in
	//		SpringArmComponenet->TargetArmLength -= ZoomSpeed;
	//		if (SpringArmComponenet->TargetArmLength < MinZoom) {
	//			SpringArmComponenet->TargetArmLength = MinZoom;
	//		}
	//	}
	//	ZoomPinchPreTick = Value;
	//}
}

void ATinyMetroCamera::Touch1Press() {
	Touch1Pressed = true;
	Touch1PressTime = 0.0f;
	CurrentRotation = SpringArmComponenet->GetRelativeRotation();

	if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
	bool tmp;
	PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, Touch1InitPosition.X, Touch1InitPosition.Y, tmp);
	if (UGameplayStatics::GetPlatformName() == TEXT("Windows")) {
		PlayerControllerRef->GetMousePosition(Touch1InitPosition.X, Touch1InitPosition.Y);
	}
	
}

void ATinyMetroCamera::Touch1Release() {
	Touch1Pressed = false;
	IsRotationMode = false;
	IsMoveMode = false;
}

void ATinyMetroCamera::Touch1DoubleClick() {
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, TEXT("Touch1 DoubleClick"));
}

void ATinyMetroCamera::Touch1Axis(float Axis) {
	if (Touch1Pressed && !Touch2Pressed) {
		// Get finger position
		FVector2D currentTouchPositon;
		if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
		bool tmp;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, currentTouchPositon.X, currentTouchPositon.Y, tmp);
		if (UGameplayStatics::GetPlatformName() == TEXT("Windows")) {
			PlayerControllerRef->GetMousePosition(currentTouchPositon.X, currentTouchPositon.Y);
		}
		auto distance = currentTouchPositon - Touch1InitPosition;

		
		// Move mode On
		if (!IsRotationMode && UKismetMathLibrary::Distance2D(currentTouchPositon, Touch1InitPosition) >= 10) {
			IsMoveMode = true;
		}

		// Rotation mode On
		if (!IsMoveMode && Touch1PressTime >= 1.0f) {
			IsRotationMode = true;
		}

		if (IsRotationMode) {
			// Rotation

			// Rotate Y
			float normalizeDistanceY = UKismetMathLibrary::NormalizeToRange(distance.Y, (float)-ScreenY, (float)ScreenY);
			double rotationValueY = UKismetMathLibrary::Lerp(-90, 90, normalizeDistanceY);
			double clampPitch = UKismetMathLibrary::ClampAngle(CurrentRotation.Pitch + rotationValueY * CameraRotationSpeedY, MinRotationAxisY, MaxRotationAxisY);

			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Touch1Axis : %f %f %f %f %f %f"), (float)ScreenY, currentTouchPositon.Y, normalizeDistanceY, rotationValueY, clampPitch, CurrentRotation.Pitch));

			// Rotation X
			float normalizeDistanceX = UKismetMathLibrary::NormalizeToRange(distance.X, (float)-ScreenX, (float)ScreenX);
			float rotationValueX = UKismetMathLibrary::Lerp(-180, 180, normalizeDistanceX);
			double clampYaw = UKismetMathLibrary::ClampAngle(CurrentRotation.Yaw + rotationValueX, -180, 180);
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, FString::Printf(TEXT("Touch1Axis : %f %f %f %f %f %f"), (float)ScreenX, currentTouchPositon.X, normalizeDistanceX, rotationValueX, clampYaw, CurrentRotation.Yaw));

			SpringArmComponenet->SetRelativeRotation(FRotator(clampPitch, CurrentRotation.Yaw + rotationValueX * CameraRotationSpeedX, 0.0f));
		} 
		if(IsMoveMode) {
			if (CameraMoveEnable) {
				// Move
				FVector curLocation = GetActorLocation();
				FRotator curRotation = SpringArmComponenet->GetRelativeRotation();
				curRotation.Pitch = 0;
				curRotation.Roll = 0;
				FVector addLocation = UKismetMathLibrary::Quat_RotateVector(curRotation.Quaternion(), FVector(-distance, 0));
				
				FVector newLocation(
					UKismetMathLibrary::FClamp(curLocation.X + addLocation.X * CameraMoveSpeedX, CameraMoveBoundaryMinX, CameraMoveBoundaryMaxX),
					UKismetMathLibrary::FClamp(curLocation.Y + addLocation.Y * CameraMoveSpeedY, CameraMoveBoundaryMinY, CameraMoveBoundaryMaxY),
					curLocation.Z
				);

				SetActorLocation(newLocation);
				//AddActorWorldOffset(FVector(distance.X * CameraMoveSpeedX, distance.Y * CameraMoveSpeedY, 0));
			}

		}
	}
}

void ATinyMetroCamera::Touch2Press() {
	Touch2Pressed = true;
	bool tmp;
	FVector2D touch1Position, touch2Position;
	PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, touch1Position.X, touch1Position.Y, tmp);
	PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch2, touch2Position.X, touch2Position.Y, tmp);
	Touch2StartDistance = UKismetMathLibrary::Distance2D(touch1Position, touch2Position);
	CurrentZoom = SpringArmComponenet->TargetArmLength;
}

void ATinyMetroCamera::Touch2Release() {
	Touch2Pressed = false;
}

void ATinyMetroCamera::Touch2Axis(float Axis) {
	if (Touch2Pressed && !IsMoveMode && !IsRotationMode) {
		bool tmp;
		FVector2D touch1Position, touch2Position;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, touch1Position.X, touch1Position.Y, tmp);
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch2, touch2Position.X, touch2Position.Y, tmp);

		auto curDistance = UKismetMathLibrary::Distance2D(touch1Position, touch2Position);
		double newCameraDistance = UKismetMathLibrary::FClamp(
			CurrentZoom + (Touch2StartDistance - curDistance) * ZoomSpeed,
			MinZoom,
			MaxZoom
		);
		
		SpringArmComponenet->TargetArmLength = newCameraDistance;
	}
}

// Called every frame
void ATinyMetroCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PlayerControllerRef->GetMousePosition(MousePosition.X, MousePosition.Y);
	//UE_LOG(LogTemp, Log, TEXT("X : %f / %d, Y : %f / %d"), MousePosition.X, ScreenX, MousePosition.Y, ScreenY);

	/*double TouchX1 = 0, TouchY1 = 0;
	double TouchX2 = 0, TouchY2 = 0;
	bool CurrentPress1, CurrentPress2;
	PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, TouchX1, TouchY1, CurrentPress1);
	
	PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch2, TouchX2, TouchY2, CurrentPress2);
	FString msg = "Tick : ";
	msg += CurrentPress1 ? "True " : "False ";
	msg += FString::SanitizeFloat(TouchX1) + " " + FString::SanitizeFloat(TouchY1);
	msg += " | ";
	msg += CurrentPress2 ? "True " : "False ";
	msg += FString::SanitizeFloat(TouchX2) + " " + FString::SanitizeFloat(TouchY2);*/

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, msg);

	//AddActorWorldRotation(FRotator(0.0f, 0.0f, 1.0f * DeltaTime).Quaternion());

	if (Touch1Pressed && !Touch2Pressed) {
		Touch1PressTime += DeltaTime;
	}
	
}

// Called to bind functionality to input
void ATinyMetroCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//PlayerInputComponent->BindAction(TEXT("Right Mouse Button"), IE_Pressed, this, &ATinyMetroCamera::CameraRotationOn);
	//PlayerInputComponent->BindAction(TEXT("Right Mouse Button"), IE_Released, this, &ATinyMetroCamera::CameraRotationOff);

	//// Bind wheel mapping (Camera zoom in, out)
	//PlayerInputComponent->BindAxis(TEXT("Mouse Wheel"), this, &ATinyMetroCamera::MouseWheel);
	//// Bind wheel mapping (Camera zoom in, out)
	//PlayerInputComponent->BindAxis(TEXT("Pinch Zoom"), this, &ATinyMetroCamera::PinchZoom);

	//PlayerInputComponent->BindAxis(TEXT("Mouse X"), this, &ATinyMetroCamera::MouseX);
	//PlayerInputComponent->BindAxis(TEXT("Mouse Y"), this, &ATinyMetroCamera::MouseY);

	PlayerInputComponent->BindAction(TEXT("Action Touch1"), IE_Pressed, this, &ATinyMetroCamera::Touch1Press);
	PlayerInputComponent->BindAction(TEXT("Action Touch1"), IE_Released, this, &ATinyMetroCamera::Touch1Release);
	PlayerInputComponent->BindAction(TEXT("Action Touch1"), IE_DoubleClick, this, &ATinyMetroCamera::Touch1DoubleClick);

	PlayerInputComponent->BindAction(TEXT("Action Touch2"), IE_Pressed, this, &ATinyMetroCamera::Touch2Press);
	PlayerInputComponent->BindAction(TEXT("Action Touch2"), IE_Released, this, &ATinyMetroCamera::Touch2Release);

	PlayerInputComponent->BindAxis(TEXT("Axis Touch1"), this, &ATinyMetroCamera::Touch1Axis);
	PlayerInputComponent->BindAxis(TEXT("Axis Touch2"), this, &ATinyMetroCamera::Touch2Axis);

}

