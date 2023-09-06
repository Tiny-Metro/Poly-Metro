// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/TinyMetroCamera.h"
#include <Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Engine/Classes/Camera/CameraComponent.h>

// Sets default values
ATinyMetroCamera::ATinyMetroCamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Rootcomponent"));

	SpringArmComponenet = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
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

	RootComponent->SetWorldRotation(FRotator(0.0f, -90.0f, 0.0f));

	SpringArmComponenet->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f));
	SpringArmComponenet->TargetArmLength = ZoomScale;
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
	if (IsCameraRotation) {
		CameraComponent->AddLocalRotation(FRotator(0.0f, Axis, 0.0f).Quaternion());
		SpringArmComponenet->AddLocalRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
	}
}

void ATinyMetroCamera::MouseY(float Axis) {
	if (Axis != 0.0f) UE_LOG(LogTemp, Log, TEXT("MouseY : %f"), Axis);
	if (IsCameraRotation) {
		SpringArmComponenet->AddLocalRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
		//CameraComponent->AddLocalRotation(FRotator(Axis, 0.0f, 0.0f).Quaternion());
	}
}

void ATinyMetroCamera::CameraRotationOn() {
	IsCameraRotation = true;
}

void ATinyMetroCamera::CameraRotationOff() {
	IsCameraRotation = false;
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
	if (Value != 0.0f) {
		SpringArmComponenet->TargetArmLength *= Value;
	}
}

// Called every frame
void ATinyMetroCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//PlayerControllerRef->GetMousePosition(MousePosition.X, MousePosition.Y);
	//UE_LOG(LogTemp, Log, TEXT("X : %f / %d, Y : %f / %d"), MousePosition.X, ScreenX, MousePosition.Y, ScreenY);

	
}

// Called to bind functionality to input
void ATinyMetroCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Right Mouse Button"), IE_Pressed, this, &ATinyMetroCamera::CameraRotationOn);
	PlayerInputComponent->BindAction(TEXT("Right Mouse Button"), IE_Released, this, &ATinyMetroCamera::CameraRotationOff);

	// Bind wheel mapping (Camera zoom in, out)
	PlayerInputComponent->BindAxis(TEXT("Mouse Wheel"), this, &ATinyMetroCamera::MouseWheel);
	// Bind wheel mapping (Camera zoom in, out)
	PlayerInputComponent->BindAxis(TEXT("Pinch Zoom"), this, &ATinyMetroCamera::PinchZoom);

	PlayerInputComponent->BindAxis(TEXT("Mouse X"), this, &ATinyMetroCamera::MouseX);
	PlayerInputComponent->BindAxis(TEXT("Mouse Y"), this, &ATinyMetroCamera::MouseY);

}

