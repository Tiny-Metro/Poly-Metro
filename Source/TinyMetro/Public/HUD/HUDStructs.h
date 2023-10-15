// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "HUDEnums.h"

#include "HUDStructs.generated.h"

USTRUCT(BlueprintType)
struct TINYMETRO_API FLanguagePair
{
    GENERATED_USTRUCT_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString Korean;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString English;
};

USTRUCT(BlueprintType)
struct TINYMETRO_API FTextComponent
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* TextWidgetREF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EHUDText TextEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETextType TextType;
};

USTRUCT(BlueprintType)
struct TINYMETRO_API FImageComponent
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UImage* ImageREF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EHUDImage ImageEnum;
};