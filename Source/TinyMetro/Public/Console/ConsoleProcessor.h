// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ConsoleProcessor.generated.h"

UCLASS()
class TINYMETRO_API AConsoleProcessor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConsoleProcessor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

};
