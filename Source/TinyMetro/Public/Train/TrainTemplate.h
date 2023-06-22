// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TrainDirection.h"
#include "../Station/StationInfo.h"
#include "TrainTemplate.generated.h"

// Train speed
#define TRAIN_DEFAULT_SPEED 300.0f
#define TRAIN_UPGRADE_SPEED 400.0f

// Distance between Train and Subtrain
#define DIST_TRAIN_SUBTRAIN 480.0f
#define DIST_TRAIN_UP_SUBTRAIN 550.0f
#define DIST_UP_TRAIN_SUBTRAIN 550.0f
#define DIST_UP_TRAIN_UP_SUBTRAIN 630.0f
#define DIST_SUBTRAIN_SUBTRAIN 410.0f
#define DIST_SUBTRAIN_UP_SUBTRAIN 450.0f
#define DIST_UP_SUBTRAIN_SUBTRAIN 460.0f
#define DIST_UP_SUBTRAIN_UP_SUBTRAIN 550.0f

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
	/*UFUNCTION()
	void InitTrainMesh();*/
	UFUNCTION()
	void TrainMaterialDeferred();
	/*UFUNCTION()
	void TrainMeshDeferred();*/
	UFUNCTION()
	void InitPassengerMaterial();

	UFUNCTION()
	void SetTrainSpeed(float Speed);
	
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
	bool GetIsUpgrade() const;
	UFUNCTION(BlueprintCallable)
	virtual void Test();
	UFUNCTION(BlueprintCallable)
	virtual FVector GetNextTrainDestination(FVector CurLocation);
	UFUNCTION(BlueprintCallable)
	AActor* ConvertMousePositionToWorldLocation(FVector& WorldLocation);
	UFUNCTION(BlueprintCallable)
	virtual void SetTrainMaterial(class ALane* Lane);
	UFUNCTION(BlueprintCallable)
	virtual bool IsPassengerSlotFull();
	UFUNCTION()
	virtual int32 GetValidSeatCount() const;
	UFUNCTION()
	virtual bool AddPassenger(class UPassenger* P);
	UFUNCTION()
	virtual void UpdatePassengerSlot();
	UFUNCTION()
	virtual void UpdateTrainMesh();
	UFUNCTION(BlueprintCallable)
	virtual bool CanUpgrade() const;


	UFUNCTION(BlueprintCallable)
	virtual void ServiceStart(FVector StartLocation, class ALane* Lane, UPARAM(DisplayName = "Destination")class AStation* D);


	UFUNCTION(BlueprintCallable)
	virtual void DespawnTrain();
	UFUNCTION(BlueprintCallable)
	void DropPassenger();
	
	virtual void GetOffPassenger(class AStation* Station, bool* Success);

	UFUNCTION()
	FStationInfo GetCurrentStation() const;
	UFUNCTION()
	void SetCurrentStation(FStationInfo Info);
	UFUNCTION()
	FStationInfo GetNextStation() const;
	UFUNCTION()
	void SetNextStation(FStationInfo Info);

	// Click & Release
	UFUNCTION()
	virtual void TrainOnPressed(AActor* Target, FKey ButtonPressed);
	UFUNCTION()
	virtual void TrainOnReleased(AActor* Target, FKey ButtonPressed);

	UFUNCTION()
	void SetDespawnNextStation();

	UFUNCTION()
	void SetTrainInfoWidget(class UTrainInfoWidget* Widget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 TrainId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 ServiceLaneId;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	class ALane* LaneRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ALaneManager* LaneManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AGridManager* GridManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class AStationManager* StationManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATinyMetroPlayerState* PlayerStateRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	class ATrainManager* TrainManagerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TrainDirection Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TMap<int32, class UPassenger*> Passenger;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float CurrentSpeed = 0.0f;

	UPROPERTY(BlueprintReadWrite)
	TArray<AActor*> LineTraceIgnoreActors;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsUpgrade = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	bool IsActorDragged = false;
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
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	class AStation* Destination;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 Fare = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<UStaticMeshComponent*> PassengerMeshComponent;

	TArray<FVector> PassengerMeshPosition = {};
	TArray<FVector> PassengerMeshPositionUpgrade = {};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	FRotator PassengerMeshRotation = FRotator(0.0f, -90.0f, 20.0f);

	UPROPERTY(BlueprintReadWrite)
	float TouchTime = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	bool TouchInput = false;
	UPROPERTY(BlueprintReadWrite)
	float TrainSafeDistance = 250.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Info")
	float TrainZAxis;

	UPROPERTY(BlueprintReadWrite)
	FVector MouseToWorldLocation;
	UPROPERTY(BlueprintReadWrite)
	AActor* MouseToWorldActor;

	UPROPERTY(BlueprintReadOnly, Category = "Info")
	bool DeferredDespawn = false;

	// Info widget
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTrainInfoWidget* TrainInfoWidget;
	
	// Check click & longclick
	bool IsSingleClick = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* TrainMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSoftObjectPath> TrainMaterialPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> TrainMaterial;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString TrainDefaultMaterialPath = "Material'/Game/Train/TrainMatrial/M_DefaultTrain.M_DefaultTrain'";
	/*UPROPERTY(Config, VisibleAnywhere, BlueprintReadOnly)
	TArray<FSoftObjectPath> TrainMeshPath;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> TrainMesh;*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCharacterMovementComponent* TrainMovement;

	// Mesh, Material paths (Passenger)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UStaticMesh*> PassengerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<UMaterial*> PassengerMaterial;
	UPROPERTY()
	TArray<FString> PassengerMeshPath = {
		TEXT("StaticMesh'/Game/Passenger/PassengerMesh/SM_PassengerNone.SM_PassengerNone'"),
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
