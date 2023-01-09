// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserHUD.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UUserHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateMoney();
	virtual void UpdateMoney_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateCurrentEvent();
	virtual void UpdateCurrentEvent_Implementation();

};
