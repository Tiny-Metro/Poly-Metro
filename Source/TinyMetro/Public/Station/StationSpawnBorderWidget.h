// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StationSpawnBorderWidget.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UStationSpawnBorderWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AlarmTop();
	virtual void AlarmTop_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AlarmBottom();
	virtual void AlarmBottom_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AlarmLeft();
	virtual void AlarmLeft_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void AlarmRight();
	virtual void AlarmRight_Implementation();
};
