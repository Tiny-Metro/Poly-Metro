#include "Camera/RtsCamera.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ARtsCamera::ARtsCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	CustomCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, FName("RtsCamera_CollisionComponent0"));
	CustomCollisionComponent->InitSphereRadius(100.0f);
	CustomCollisionComponent->CanCharacterStepUpOn = ECB_No;
	CustomCollisionComponent->SetCanEverAffectNavigation(false);
	RootComponent = CustomCollisionComponent;
	MovementComponent = ObjectInitializer.CreateDefaultSubobject<UFloatingPawnMovement>(this, FName("RtsCameran_MovementComponent0"));
	MovementComponent->UpdatedComponent = CustomCollisionComponent;
}

void ARtsCamera::SetupPlayerInputComponent(UInputComponent* Inputelemental) 
{
	check(Inputelemental);
	Inputelemental->BindAxis("Mouse Wheel", this, &ARtsCamera::ZoomIn);
	Inputelemental->BindAction("Rotate Camera", IE_Pressed, this, &ARtsCamera::EngageRotation);
	Inputelemental->BindAction("Rotate Camera", IE_Released, this, &ARtsCamera::DisengageRotation);
}

void ARtsCamera::Tick(float DeltaSeconds)
{ 
	if (playerController == nullptr) 
	{
		playerController = Cast<APlayerController>(GetController());
	}

	if (playerController->GetMousePosition(mouseLocation.X, mouseLocation.Y)) 
	{
		if (rotationMode)
		{
			DeltaMouse = mouseScreenLocationGrab - mouseLocation;
			DeltaMouse *= RotationSpeed * DeltaSeconds;
			NewRotation = GetActorRotation();
			NewRotation.Roll += DeltaMouse.X;
			NewRotation.Pitch += DeltaMouse.Y;
			SetActorRotation(NewRotation);
			mouseScreenLocationGrab = mouseLocation;
		}
			
		else
		{
			if (mouseLocation.X < screenWidth * ScreenEdgePanZonePercent / 100)
				PanLeft();
			else if (mouseLocation.X > screenWidth * (1 - ScreenEdgePanZonePercent / 100))
				PanRight();
			if (mouseLocation.Y < screenHeight * ScreenEdgePanZonePercent / 100)
				PanForward();
			else if (mouseLocation.Y > screenHeight * (1 - ScreenEdgePanZonePercent / 100))
				PanBackward();
		}
	}		
}


void ARtsCamera::BeginPlay()
{
	Super::BeginPlay();
	playerController = Cast<APlayerController>(GetController());
	CustomCollisionComponent->SetWorldLocation(initLocation);
	CustomCollisionComponent->SetWorldRotation(initRotation);
	playerController->GetViewportSize(screenWidth, screenHeight);
}

void ARtsCamera::ChangePitch(const float magnitude)
{
	if (magnitude != 0 && rotationMode && playerController)
		if (magnitude < 0 && GetControlRotation().Pitch < MaxPitch || magnitude > 0 && GetControlRotation().Pitch > MinPitch)
			AddControllerPitchInput(magnitude * PitchChangeRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::Yaw(const float magnitude)
{
	if (magnitude != 0 && rotationMode && playerController)
	{
		AddControllerYawInput(magnitude * YawRate * GetWorld()->GetDeltaSeconds());
	}
}

void ARtsCamera::Orbit(const float magnitude)
{
	if (magnitude != 0 && rotationMode && playerController)
	{
		AddControllerYawInput(magnitude * YawRate * GetWorld()->GetDeltaSeconds());
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), magnitude * -(YawRate / 4) * GetWorld()->GetDeltaSeconds());
	}
}

void ARtsCamera::PanForward()
{	
	if (!rotationMode && playerController)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), PanRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::PanBackward()
{
	if (!rotationMode && playerController)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), -PanRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::PanRight()
{
	if ( !rotationMode && playerController)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), PanRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::PanLeft()
{
	if ( !rotationMode && playerController)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), -PanRate * GetWorld()->GetDeltaSeconds());
}


void ARtsCamera::ZoomIn(const float magnitude)
{
	if (magnitude != 0 && !rotationMode && playerController)
		if (magnitude > 0 && GetActorLocation().Z > MinZoom || magnitude < 0 && GetActorLocation().Z < MaxZoom)
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Z),  magnitude * -ZoomRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::EngageRotation()
{
	if (playerController)
	{
		rotationMode = true;
		playerController->bShowMouseCursor = false;  
		playerController->bEnableClickEvents = false;
		playerController->bEnableMouseOverEvents = false;
		InitialRotation = GetActorRotation();		
	}
}


void ARtsCamera::DisengageRotation()
{
	if (rotationMode && playerController)
	{
		rotationMode = false;
		playerController->bShowMouseCursor = true;  
		playerController->bEnableClickEvents = true;
		playerController->bEnableMouseOverEvents = true;
		SetActorRotation(InitialRotation);  
	}
}


void ARtsCamera::JumpTo(FVector NewLocation)
{
	FVector NewCameraLocation = FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z);
	SetActorLocation(NewCameraLocation);
}

