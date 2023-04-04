// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Gauge.generated.h"

UCLASS()
class TINYMETRO_API AGauge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGauge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable)
	void InitGauge(UStaticMeshComponent* staticMeshComponent, UMaterialInterface* parentMaterial);

	UFUNCTION(BlueprintCallable)
	void SetGaugePer(float gaugePer);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	FName ScalarParamName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	float GaugePer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMeshComponent* GaugeMesh;

};
