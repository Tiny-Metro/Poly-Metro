// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PolicyData.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct TINYMETRO_API FPolicyData
{
	GENERATED_USTRUCT_BODY()


public:
	FPolicyData();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		int ServiceCostLevel = 3;      // 서비스 비용 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HandicappedSeat = false;  // 교통약자 좌석 배치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasCCTV = false;          // CCTV 설치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasElevator = false;      // 엘레베이터 설치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasBicycle = false;       // 자전거 탑승 허가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasTransfer = false;      // 환승 시스템

};
