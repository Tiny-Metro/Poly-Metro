// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class TINYMETRO_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()




protected:
	UPROPERTY(Config, EditAnywhere, BlueprintReadWrite)
	TArray <FString> ExplainText;
	
};
