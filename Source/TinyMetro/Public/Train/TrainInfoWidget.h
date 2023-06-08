// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TrainInfoWidget.generated.h"

/**
 * Visible when Train, Subtrain clicked
 * Show Train's information
 */
UCLASS()
class TINYMETRO_API UTrainInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowWidget(class ATrainTemplate* Train);
	virtual void ShowWidget_Implementation(class ATrainTemplate* Train);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ATrainTemplate* TrainRef;

};
