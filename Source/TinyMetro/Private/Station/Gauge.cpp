// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/Gauge.h"

// Sets default values
AGauge::AGauge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGauge::BeginPlay()
{
	Super::BeginPlay();
}



void AGauge::InitGauge(UStaticMeshComponent* staticMeshComponent, UMaterialInterface* parentMaterial)
{
	// Create a dynamic material instance
	DynamicMaterial = UMaterialInstanceDynamic::Create(parentMaterial, this);

	// Set the GaugePer value
	GaugePer = 0.0f; // Replace this with the value you want to set
	DynamicMaterial->SetScalarParameterValue("Gauge", GaugePer);

	GaugeMesh = staticMeshComponent;
	GaugeMesh->SetMaterial(0, DynamicMaterial);
}

void AGauge::SetGaugePer(float guagePer) {
	GaugePer = guagePer; // Replace this with the value you want to set
	DynamicMaterial->SetScalarParameterValue("Gauge", GaugePer);
}

// Called every frame
void AGauge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
//	setGauge(ScalarValue + DeltaTime / 10.0f);
}
