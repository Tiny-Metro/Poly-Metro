// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainDirection.h"
#include "TrainTemplate.generated.h"

UCLASS(Config = Game)
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

	UFUNCTION()
	void InitTrainMaterial();
	UFUNCTION()
	void InitTrainMesh();
	UFUNCTION()
	void TrainMaterialDeferred();
	UFUNCTION()
	void TrainMeshDeferred();
	
	UFUNCTION(BlueprintCallable)
	void SetTrainId(int32 Id);
	UFUNCTION(BlueprintCallable)
	int32 GetTrainId() const;
	UFUNCTION(BlueprintCallable)
	void SetServiceLaneId(int32 Id);
	UFUNCTION(BlueprintCallable)
	int32 GetServiceLaneId() const;
	UFUNCTION(BlueprintCallable)
	void SetTrainDirection(TrainDirection Dir);
	UFUNCTION(BlueprintCallable)
	TrainDirection GetTrainDirection() const;
	UFUNCTION(BlueprintCallable)
	virtual void Upgrade();
	UFUNCTION(BlueprintCallable)
	virtual void Test();
	UFUNCTION(BlueprintCallable)
	virtual FVector GetNextTrainDestination(FVector CurLocation);
	UFUNCTION(BlueprintCallable)
	FVector ConvertMousePositionToWorldLocation();
	UFUNCTION(BlueprintCallable)
	virtual void SetTrainMaterial(class ALane* Lane);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 TrainId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 ServiceLaneId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ALaneManager* LaneManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AGridManager* GridManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TrainDirection Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsUpgrade = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsActorDragged;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float OnPressedTime;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float LongClickInterval;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float TotalTravel;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrainMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSoftObjectPath> TrainMaterialPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> TrainMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString TrainDefaultMaterialPath = "Material'/Game/Train/TrainMatrial/M_DefaultTrain.M_DefaultTrain'";
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly)
	TArray<FSoftObjectPath> TrainMeshPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> TrainMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCharacterMovementComponent* TrainMovement;
};
