#include "Camera/RtsCamera.h"
#include "Engine/Classes/Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ARtsCamera::ARtsCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

	CustomCollisionComponent = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, FName("RtsCamera_CollisionComponent0"));
	CustomCollisionComponent->InitSphereRadius(100.0f);
	CustomCollisionComponent->CanCharacterStepUpOn = ECB_No;
	CustomCollisionComponent->SetCanEverAffectNavigation(false);
	CustomCollisionComponent->SetRelativeLocation(FVector(0, 0, 6000.f)); //초기위치,높이 설정
	CustomCollisionComponent->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
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
				int32 screenWidth, screenHeight;
				playerController->GetViewportSize(screenWidth, screenHeight);

				if (rotationMode)
				{
					FVector2D DeltaMouse = mouseScreenLocationGrab - mouseLocation;
					float RotationSpeed = 1.0f;  // 회전 속도 조절
					DeltaMouse *= RotationSpeed * GetWorld()->GetDeltaSeconds();

					FRotator NewRotation = GetActorRotation();
					NewRotation.Roll += DeltaMouse.X;
					NewRotation.Pitch += DeltaMouse.Y;
					SetActorRotation(NewRotation);

					mouseScreenLocationGrab = mouseLocation;
				}
				else
				{
					FVector MovementInput = FVector::ZeroVector;

					if (mouseLocation.X < screenWidth * ScreenEdgePanZonePercent / 100)
						MovementInput.Y -= 1;
					else if (mouseLocation.X > screenWidth * (1 - ScreenEdgePanZonePercent / 100))
						MovementInput.Y += 1;

					if (mouseLocation.Y < screenHeight * ScreenEdgePanZonePercent / 100)
						MovementInput.X += 1;
					else if (mouseLocation.Y > screenHeight * (1 - ScreenEdgePanZonePercent / 100))
						MovementInput.X -= 1;

					MovementInput.Normalize();
					PanRight(MovementInput.Y);
					PanForward(MovementInput.X);
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
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), magnitude * 4000 * GetWorld()->GetDeltaSeconds());
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
			AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::X), 1000000000000 * magnitude * ZoomRate * GetWorld()->GetDeltaSeconds());
}




void ARtsCamera::EngageRotation()
{
	if (Controller)
	{
		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			if (playerController->IsInputKeyDown(EKeys::RightMouseButton))  // 오른쪽 마우스 버튼이 눌려있는지 확인
			{
				rotationMode = true;
				playerController->bShowMouseCursor = false;  // 마우스 커서 숨김
				playerController->bEnableClickEvents = false;
				playerController->bEnableMouseOverEvents = false;

				// 초기 회전값 저장
				InitialRotation = GetActorRotation();
			}
		}
	}
}


void ARtsCamera::DisengageRotation()
{
	if (rotationMode)
	{
		rotationMode = false;

		APlayerController* playerController = Cast<APlayerController>(GetController());
		if (playerController != nullptr)
		{
			playerController->bShowMouseCursor = true;  // 마우스 커서 표시
			playerController->bEnableClickEvents = true;
			playerController->bEnableMouseOverEvents = true;

			// 초기 회전값으로 초기화
			SetActorRotation(InitialRotation);  // InitialRotation은 클래스 멤버 변수로 선언되어야 합니다.
		}
	}
}

void ARtsCamera::JumpTo(FVector NewLocation)
{
	FVector NewCameraLocation = FVector(NewLocation.X, NewLocation.Y, GetActorLocation().Z);
	SetActorLocation(NewCameraLocation);
}


