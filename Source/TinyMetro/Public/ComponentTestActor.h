// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ComponentTestActor.generated.h"

UCLASS()
class TINYMETRO_API AComponentTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AComponentTestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Root")
	USceneComponent* DefaultRoot = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Test")
	USceneComponent* MyScene = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
	UStaticMesh* MyMesh = nullptr;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TArray<FVector3d> meshPoints;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	TArray<UStaticMeshComponent*> MySMs;

};
