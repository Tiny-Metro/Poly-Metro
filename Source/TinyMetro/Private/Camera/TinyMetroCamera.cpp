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

// Called every frame
void ATinyMetroCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATinyMetroCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Mouse Wheel"), this, &ATinyMetroCamera::MouseWheel);

}

