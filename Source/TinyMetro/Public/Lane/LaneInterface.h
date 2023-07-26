// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LaneInterface.generated.h" // This is necessary for UCLASS(), USTRUCT(), UENUM(), etc.

UINTERFACE(BlueprintType)
class TINYMETRO_API ULaneInterface : public UInterface
{
    GENERATED_BODY()
};

class TINYMETRO_API ILaneInterface
{
    GENERATED_BODY()

public:
    virtual void PopHandle(bool IsUp) = 0;
};
