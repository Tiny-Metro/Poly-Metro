// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/SplineMeshComponent.h"
#include "MeshComponentArray.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FMeshComponentArray{

    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<USplineMeshComponent*> MeshArray;
};
