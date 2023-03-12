// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainDirection.h"
#include "../Station/StationInfo.h"
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
	void UpdatePassengerMesh();

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
	UFUNCTION()
	void InitPassengerMaterial();
	
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
	UFUNCTION(BlueprintCallable)
	virtual bool IsPassengerSlotFull();
	UFUNCTION()
	virtual int32 GetValidSeatCount() const;
	UFUNCTION()
	virtual bool AddPassenger(class UPassenger* P);


	UFUNCTION(BlueprintCallable)
	virtual void DespawnTrain();
	UFUNCTION(BlueprintCallable)
	void DropPassenger();

	UFUNCTION()
	FStationInfo GetCurrentStation() const;
	UFUNCTION()
	FStationInfo GetNextStation() const;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 TrainId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 ServiceLaneId;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ALaneManager* LaneManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AGridManager* GridManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AStationManager* StationManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TrainDirection Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TMap<int32, class UPassenger*> Passenger;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsUpgrade = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsActorDragged;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float OnPressedTime;
	UPROPERTY(Config, VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float LongClickInterval;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float TotalTravel = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 TotalPassenger = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 MaxPassengerSlotUpgrade = 8;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 MaxPassengerSlot = 6;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 CurrentPassengerSlot = 6;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	FStationInfo CurrentStation;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	FStationInfo NextStation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<UStaticMeshComponent*> PassengerMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FVector> PassengerMeshPosition; 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FVector> PassengerMeshPositionUpgrade;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	FRotator PassengerMeshRotation = FRotator(0.0f, -90.0f, 20.0f);

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

	// Mesh, Material paths (Passenger)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> PassengerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> PassengerMaterial;
	UPROPERTY()
	TArray<FString> PassengerMeshPath = {
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerCircle.SM_PassengerCircle'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerTriangle.SM_PassengerTriangle'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerRectangle.SM_PassengerRectangle'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerCross.SM_PassengerCross'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerRhombus.SM_PassengerRhombus'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerOval.SM_PassengerOval'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerDiamond.SM_PassengerDiamond'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerPentagon.SM_PassengerPentagon'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerStar.SM_PassengerStar'"),
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerFan.SM_PassengerFan'")
	};
};
