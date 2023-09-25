// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/TinyMetroCamera.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "SaveSystem/TMSaveManager.h"
#include "Camera/TinyMetroCameraSaveGame.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include <Kismet/GameplayStatics.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
ATinyMetroCamera::ATinyMetroCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

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

	if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
	PlayerControllerRef->GetViewportSize(ScreenX, ScreenY);

	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}

	GEngine->GameViewport->Viewport->ViewportResizedEvent.AddUObject(this, &ATinyMetroCamera::InitViewport);
	SpringArmComponenet->TargetArmLength = CurrentZoom;

	Load();

	SaveManagerRef->SaveTask.AddDynamic(this, &ATinyMetroCamera::Save);
}

void ATinyMetroCamera::InitViewport(FViewport* Viewport, uint32 unused) {
	if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
	PlayerControllerRef->GetViewportSize(ScreenX, ScreenY);
}

void ATinyMetroCamera::MouseWheel(float Axis) {
	double newZoom = UKismetMathLibrary::FClamp(SpringArmComponenet->TargetArmLength + -Axis * ZoomSpeed, MinZoom, MaxZoom);
	SpringArmComponenet->TargetArmLength = newZoom;
}

void ATinyMetroCamera::CameraMoveX(float Axis) {
	// Axis + : Move right
	// Axis - : Move left
	if (CameraMoveEnable) {
		FRotator curRotation = GetActorRotation();
		curRotation.Pitch = 0;
		curRotation.Roll = 0;
		curRotation.Yaw += 90;
		FVector curLocation = GetActorLocation();
		FVector addLocation = UKismetMathLibrary::Quat_RotateVector(curRotation.Quaternion(), FVector(Axis, 0, 0));

		FVector newLocation(
			UKismetMathLibrary::FClamp(curLocation.X + addLocation.X * CameraMoveSpeedX, CameraMoveBoundaryMinX, CameraMoveBoundaryMaxX),
			UKismetMathLibrary::FClamp(curLocation.Y + addLocation.Y * CameraMoveSpeedY, CameraMoveBoundaryMinY, CameraMoveBoundaryMaxY),
			curLocation.Z
		);

		SetActorLocation(newLocation);
	}
}

void ATinyMetroCamera::CameraMoveY(float Axis) {
	// Axis + : Move down
	// Axis - : Move up
	if (CameraMoveEnable) {
		FRotator curRotation = GetActorRotation();
		curRotation.Pitch = 0;
		curRotation.Roll = 0;
		curRotation.Yaw += 90;
		FVector curLocation = GetActorLocation();
		FVector addLocation = UKismetMathLibrary::Quat_RotateVector(curRotation.Quaternion(), FVector(0, Axis, 0));

		FVector newLocation(
			UKismetMathLibrary::FClamp(curLocation.X + addLocation.X * CameraMoveSpeedX, CameraMoveBoundaryMinX, CameraMoveBoundaryMaxX),
			UKismetMathLibrary::FClamp(curLocation.Y + addLocation.Y * CameraMoveSpeedY, CameraMoveBoundaryMinY, CameraMoveBoundaryMaxY),
			curLocation.Z
		);

		SetActorLocation(newLocation);
	}
}

void ATinyMetroCamera::CameraRotationZ(float Axis) {
	if (Axis != 0 && !IsResetRotation) {
		AddActorWorldRotation(FRotator(0.0, Axis * CameraRotationSpeedZ, 0));
	}
}

void ATinyMetroCamera::CameraRotationY(float Axis) {
	if (Axis != 0 && !IsResetRotation) {
		FRotator curRotation = SpringArmComponenet->GetRelativeRotation();
		SpringArmComponenet->SetRelativeRotation(FRotator(
			UKismetMathLibrary::FClamp(curRotation.Pitch + Axis * CameraRotationSpeedY, MinRotationAxisY, MaxRotationAxisY),
			0,
			0)
		);
	}
}

void ATinyMetroCamera::ToggleCameraMoveEnable() {
	CameraMoveEnable = !CameraMoveEnable;
}

void ATinyMetroCamera::Touch1Press() {
	if (!IsResetRotation) {
		Touch1Pressed = true;
		Touch1PressTime = 0.0f;
		CurrentRotation = SpringArmComponenet->GetRelativeRotation();
		CurrentRotation.Yaw = GetActorRotation().Yaw;

		if (!IsValid(PlayerControllerRef)) PlayerControllerRef = Cast<APlayerController>(Controller);
		bool tmp;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, Touch1InitPosition.X, Touch1InitPosition.Y, tmp);
		if (UGameplayStatics::GetPlatformName() == TEXT("Windows")) {
			PlayerControllerRef->GetMousePosition(Touch1InitPosition.X, Touch1InitPosition.Y);
		}
	}
}

void ATinyMetroCamera::Touch1Release() {
	Touch1Pressed = false;
	IsRotationMode = false;
	IsMoveMode = false;
}

void ATinyMetroCamera::ResetRotation() {
	if (abs(GetActorRotation().Yaw) > 90) {
		// Rotate clockwise
		if (GetActorRotation().Yaw > 0) {
			TargetResetRotation = -((GetActorRotation().Yaw - 180) - 90);
		} else {
			TargetResetRotation = -90 - GetActorRotation().Yaw;
		}
	} else {
		// Rotate anti clockwise
		TargetResetRotation = -90 - GetActorRotation().Yaw;
	}
	IsResetRotation = true;
}

void ATinyMetroCamera::MoveCamera(FVector2D TargetLocation) {
	TargetMoveLocation = TargetLocation;
	TargetDistance = TargetLocation - FVector2D(GetActorLocation());
	IsAutoMovement = true;
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
		if (!IsMoveMode && Touch1PressTime >= LongTouchInterval) {
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

			SpringArmComponenet->SetRelativeRotation(FRotator(clampPitch, 0.0, 0.0));
			SetActorRotation(FRotator(0.0, CurrentRotation.Yaw + rotationValueX * CameraRotationSpeedZ, 0.0));
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
			}
		}
	}
}

void ATinyMetroCamera::Touch2Press() {
	if (!IsResetRotation) {
		Touch2Pressed = true;
		bool tmp;
		FVector2D touch1Position, touch2Position;
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch1, touch1Position.X, touch1Position.Y, tmp);
		PlayerControllerRef->GetInputTouchState(ETouchIndex::Touch2, touch2Position.X, touch2Position.Y, tmp);
		Touch2StartDistance = UKismetMathLibrary::Distance2D(touch1Position, touch2Position);
		CurrentZoom = SpringArmComponenet->TargetArmLength;
	}
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

void ATinyMetroCamera::Save() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UTinyMetroCameraSaveGame* tmp = Cast<UTinyMetroCameraSaveGame>(UGameplayStatics::CreateSaveGameObject(UTinyMetroCameraSaveGame::StaticClass()));
	tmp->Location = GetActorLocation();
	tmp->ParentRotation = GetActorRotation();
	tmp->SpringArmRotation = SpringArmComponenet->GetRelativeRotation();
	tmp->CameraDistance = SpringArmComponenet->TargetArmLength;

	SaveManagerRef->Save(tmp, SaveActorType::Camera);
}

void ATinyMetroCamera::Load() {
	if (!IsValid(SaveManagerRef)) {
		SaveManagerRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->GetSaveManager();
	}
	UTinyMetroCameraSaveGame* tmp = Cast<UTinyMetroCameraSaveGame>(SaveManagerRef->Load(SaveActorType::Camera));

	if (!IsValid(tmp)) {
		return;
	}

	SetActorLocation(tmp->Location);
	SetActorRotation(tmp->ParentRotation);
	SpringArmComponenet->SetRelativeRotation(tmp->SpringArmRotation);
	SpringArmComponenet->TargetArmLength = tmp->CameraDistance;

}

// Called every frame
void ATinyMetroCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Double click logic : Reset rotation
	if (IsResetRotation) {
		AddActorWorldRotation(FRotator(0, TargetResetRotation * DeltaTime * (1 / CameraRestSeconds), 0));
		if (UKismetMathLibrary::NearlyEqual_FloatFloat(GetActorRotation().Yaw, -90, 1)) {
			IsResetRotation = false;
		}
	}

	// Auto movement logic
	if (IsAutoMovement) {
		AddActorWorldOffset(FVector(TargetDistance * DeltaTime * (1 / CameraAutoMoveSeconds), 0));
		auto tmp = GetActorLocation();
		if (UKismetMathLibrary::NearlyEqual_TransformTransform(
			FTransform(FVector(TargetMoveLocation, 0)),
			FTransform(FVector(tmp.X, tmp.Y, 0)),
			10
		)) {
			IsAutoMovement = false;
		}
	}

	if (Touch1Pressed && !Touch2Pressed) {
		Touch1PressTime += DeltaTime;
	}
	
}

// Called to bind functionality to input
void ATinyMetroCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Action Touch1"), IE_Pressed, this, &ATinyMetroCamera::Touch1Press);
	PlayerInputComponent->BindAction(TEXT("Action Touch1"), IE_Released, this, &ATinyMetroCamera::Touch1Release);

	PlayerInputComponent->BindAction(TEXT("Action Touch2"), IE_Pressed, this, &ATinyMetroCamera::Touch2Press);
	PlayerInputComponent->BindAction(TEXT("Action Touch2"), IE_Released, this, &ATinyMetroCamera::Touch2Release);

	PlayerInputComponent->BindAxis(TEXT("Axis Touch1"), this, &ATinyMetroCamera::Touch1Axis);
	PlayerInputComponent->BindAxis(TEXT("Axis Touch2"), this, &ATinyMetroCamera::Touch2Axis);

	PlayerInputComponent->BindAction(TEXT("Reset Rotation"), IE_DoubleClick, this, &ATinyMetroCamera::ResetRotation);


	PlayerInputComponent->BindAction(TEXT("Camera Hold"), IE_Released, this, &ATinyMetroCamera::ToggleCameraMoveEnable);

	PlayerInputComponent->BindAxis(TEXT("Keyborad Move X"), this, &ATinyMetroCamera::CameraMoveX);
	PlayerInputComponent->BindAxis(TEXT("Keyborad Move Y"), this, &ATinyMetroCamera::CameraMoveY);
	PlayerInputComponent->BindAxis(TEXT("Keyboard Rotation Z"), this, &ATinyMetroCamera::CameraRotationZ);
	PlayerInputComponent->BindAxis(TEXT("Keyboard Rotation Y"), this, &ATinyMetroCamera::CameraRotationY);
	PlayerInputComponent->BindAxis(TEXT("Mouse Wheel"), this, &ATinyMetroCamera::MouseWheel);
}

