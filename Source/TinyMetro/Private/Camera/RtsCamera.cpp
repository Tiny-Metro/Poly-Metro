#include "Camera/RtsCamera.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ARtsCamera::ARtsCamera (const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	CustomCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, FName("RtsCamera_CollisionComponent0"));
	CustomCollisionComponent->InitSphereRadius(100.0f);
	CustomCollisionComponent->CanCharacterStepUpOn = ECB_No;
	CustomCollisionComponent->SetCanEverAffectNavigation(false);
	CustomCollisionComponent->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	RootComponent = CustomCollisionComponent;

	MovementComponent = ObjectInitializer.CreateDefaultSubobject<UFloatingPawnMovement>(this, FName("RtsCameran_MovementComponent0"));
	MovementComponent->UpdatedComponent = CustomCollisionComponent;
}

void ARtsCamera::SetupPlayerInputComponent(UInputComponent* Inputelemental)
{
	check(Inputelemental);

	Inputelemental->BindAxis("Pan Forward", this, &ARtsCamera::PanForward);
	Inputelemental->BindAxis("Pan Right", this, &ARtsCamera::PanRight);
	Inputelemental->BindAxis("Mouse Wheel", this, &ARtsCamera::ZoomIn);

	Inputelemental->BindAction("Rotate Camera", IE_Pressed, this, &ARtsCamera::EngageRotation);
	Inputelemental->BindAction("Rotate Camera", IE_Released, this, &ARtsCamera::DisengageRotation);
}

void ARtsCamera::Tick(float DeltaSeconds)
{
	if (Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			FVector2D mouseLocation;
			if (playerController->GetMousePosition(mouseLocation.X, mouseLocation.Y))
			{
				int32 screenWidth = 0;
				int32 screenHeight = 1000;
				playerController->GetViewportSize(screenWidth, screenHeight);

				if (rotationMode) //Main Camera rotation calls
				{
					Orbit((mouseScreenLocationGrab.X - mouseLocation.X) / screenWidth);
					ChangePitch((mouseScreenLocationGrab.Y - mouseLocation.Y) * -1 / screenWidth);
				}
				else //  Mouse on screen edge pan
				{
					const float xPanZone = screenWidth * ScreenEdgePanZonePercent / 100;
					const float yPanZone = screenHeight * ScreenEdgePanZonePercent / 100;

					if (mouseLocation.X < xPanZone)							PanRight((1 - mouseLocation.X / xPanZone) * -1);
					else if (mouseLocation.X > screenWidth - xPanZone)		PanRight(1 - (screenWidth - mouseLocation.X) / xPanZone);
					if (mouseLocation.Y < yPanZone)							PanForward(1 - mouseLocation.Y / yPanZone);
					else if (mouseLocation.Y > screenHeight - yPanZone)		PanForward((1 - (screenHeight - mouseLocation.Y) / yPanZone) * -1);

				}


			}
		}
	}
}

void ARtsCamera::ChangePitch(const float magnitude)
{
	if (magnitude != 0 && rotationMode && Controller)
		if (magnitude < 0 && GetControlRotation().Pitch < MaxPitch || magnitude > 0 && GetControlRotation().Pitch > MinPitch)
			AddControllerPitchInput(magnitude * PitchChangeRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::Yaw(const float magnitude)
{
	if (magnitude != 0 && rotationMode && Controller)
	{
		AddControllerYawInput(magnitude * YawRate * GetWorld()->GetDeltaSeconds());
	}

}

void ARtsCamera::Orbit(const float magnitude)
{
	if (magnitude != 0 && rotationMode && Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			AddControllerYawInput(magnitude * YawRate * GetWorld()->GetDeltaSeconds());
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), magnitude * -(YawRate / 4) * GetWorld()->GetDeltaSeconds());
		}
	}

}

void ARtsCamera::PanForward(const float magnitude)
{
	if (magnitude != 0 && !rotationMode && Controller)
		AddMovementInput(FRotationMatrix(FRotator(0, GetControlRotation().Yaw, 0)).GetScaledAxis(EAxis::X), magnitude * 4000 * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::PanRight(const float magnitude)
{
	if (magnitude != 0 && !rotationMode && Controller)
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), magnitude * PanRate * GetWorld()->GetDeltaSeconds());
}

void ARtsCamera::ZoomIn(const float magnitude)
{
	if (magnitude != 0 && !rotationMode && Controller)
		if (magnitude > 0 && GetActorLocation().Z > MinZoom || magnitude < 0 && GetActorLocation().Z < MaxZoom)
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), magnitude * ZoomRate * GetWorld()->GetDeltaSeconds());
}




void ARtsCamera::EngageRotation()
{
	if (Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			FVector2D mousePosition;
			if (playerController->GetMousePosition(mousePosition.X, mousePosition.Y))
			{
				mouseScreenLocationGrab = mousePosition;
				rotationMode = true;
			}
		}
	}
}

void ARtsCamera::DisengageRotation()
{
	rotationMode = false;
}



