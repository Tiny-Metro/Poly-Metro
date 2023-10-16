// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../HUD/PolyMetroWidget.h"
#include "StationInfoWidget.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API UStationInfoWidget : public UPolyMetroWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowWidget(class AStation* Station);
	virtual void ShowWidget_Implementation(class AStation* Station);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class AStation* StationRef;
};
