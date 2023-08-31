#include "Camera/RtsController.h"
#include "Camera/RtsCamera.h"


ARtsController::ARtsController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}



void ARtsController::BeginPlay()
{
	Super::BeginPlay();
}

void ARtsController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}