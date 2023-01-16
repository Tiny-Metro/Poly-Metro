// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RtsController.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ARtsController : public APlayerController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ForceCamera(FVector Location);
	virtual void ForceCamera_Implementation(FVector Location);
		
	


	
};
