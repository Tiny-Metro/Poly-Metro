#pragma once

#include "RtsCamera.h"
#include "GameFramework/Pawn.h"
#include "RtsController.generated.h"

UCLASS()
class TINYMETRO_API ARtsController : public APlayerController
{
	GENERATED_BODY()

public:
	ARtsController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};