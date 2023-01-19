// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainDirection.h"
#include "TrainTemplate.generated.h"

UCLASS()
class TINYMETRO_API ATrainTemplate : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATrainTemplate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UFUNCTION(BlueprintCallable)
	void SetTrainId(int32 Id);
	UFUNCTION(BlueprintCallable)
	void SetTrainDirection(TrainDirection Dir);
	UFUNCTION(BlueprintCallable)
	TrainDirection GetTrainDirection() const;
	UFUNCTION(BlueprintCallable)
	void Upgrade();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 TrainId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TrainDirection Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsUpgrade;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsActorDragged;
};
