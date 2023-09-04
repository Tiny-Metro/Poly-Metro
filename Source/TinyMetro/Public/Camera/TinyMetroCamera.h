// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TinyMetroCamera.generated.h"

UCLASS()
class TINYMETRO_API ATinyMetroCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATinyMetroCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void MouseWheel(float Axis);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MinZoom = 300.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float MaxZoom = 3000.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Zoom")
	float ZoomSpeed = 20.0f;

	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComponenet;
	UPROPERTY(VisibleAnyWhere, BlueprintReadWrite)
	class UCameraComponent* CameraComponent;
};
