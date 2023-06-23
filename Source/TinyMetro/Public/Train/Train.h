// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Train/TrainTemplate.h"
#include "Subtrain.h"
#include "TrainStatus.h"
#include "TrainAiController.h"
#include "Train.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class TINYMETRO_API ATrain : public ATrainTemplate
{
	GENERATED_BODY()
	
public:
	ATrain();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Test() override;

	// Called every frame
	// virtual void Tick(float DeltaTime) override;

	// TrainTemplate override function
	virtual FVector GetNextTrainDestination(FVector CurLocation) override;
	virtual void SetTrainMaterial(class ALane* Lane) override;
	virtual bool IsPassengerSlotFull() override;
	virtual void ServiceStart(FVector StartLocation, class ALane* Lane, class AStation* D) override;
	virtual void UpdatePassengerSlot() override;
	virtual void DespawnTrain() override;
	virtual void UpdateTrainMesh() override;
	virtual void Upgrade() override;
	virtual bool CanUpgrade() const override;

	// Click & Release
	virtual void TrainOnReleased(AActor* Target, FKey ButtonPressed) override;

	UFUNCTION(BlueprintCallable)
	void AddSubtrain(UPARAM(DisplayName = "Subtrains")ASubtrain* T);
	UFUNCTION()
	void ActiveMoveTest(); 
	UFUNCTION()
	void GetOnPassenger(class AStation* Station);

	virtual void GetOffPassenger(class AStation* Station, bool& Success) override;
	UFUNCTION()
	void UpdateSubtrainDistance();
	UFUNCTION()
	void DetachSubtrain(ASubtrain* T);
	UFUNCTION()
	void UpdateSubtrainSpeed();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	int32 MaxSubtrain = 4;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TArray<ASubtrain*> Subtrains;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	FVector LocationAtPreTick;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	ATrainAiController* AiControllerRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	TrainStatus Status = TrainStatus::Run;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float PassengerTransitionDelay = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Info")
	float PassengerTransitionCount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* CubeComponent;

protected:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapVolume;
	
	UPROPERTY()
	int32 RideCount = 0;
	UPROPERTY()
	int32 PassengerIndex = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FVector> PassengerMeshPosition = {
		FVector(10.0f, 55.0f, 190.0f),
		FVector(10.0f, -55.0f, 190.0f),
		FVector(-100.0f, 55.0f, 190.0f),
		FVector(-100.0f, -55.0f, 190.0f),
		FVector(-210.0f, 55.0f, 190.0f),
		FVector(-210.0f, -55.0f, 190.0f),
		FVector(-320.0f, 55.0f, 190.0f),
		FVector(-320.0f, -55.0f, 190.0f)
	};

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FVector> PassengerMeshPositionUpgrade = {
		FVector(70.0f, 55.0f, 190.0f),
		FVector(70.0f, -55.0f, 190.0f),
		FVector(-50.0f, 55.0f, 190.0f),
		FVector(-50.0f, -55.0f, 190.0f),
		FVector(-160.0f, 55.0f, 190.0f),
		FVector(-160.0f, -55.0f, 190.0f),
		FVector(-270.0f, 55.0f, 190.0f),
		FVector(-270.0f, -55.0f, 190.0f)
	};

	// Index 0 : Non upgrade mesh
	// Index 1 : Upgrade mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<FString> TrainMeshPath = {
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_Train.SM_Train'"),
		TEXT("StaticMesh'/Game/Train/TrainMesh/SM_TrainUpgrade.SM_TrainUpgrade'")
	};
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	TArray<UStaticMesh*> TrainMesh;

private:
	UPROPERTY()
	FTimerHandle GetOnHandle;
	FTimerDelegate GetOnDelegate;
	UPROPERTY()
	FTimerHandle GetOffHandle;
	FTimerDelegate GetOffDelegate;


};
