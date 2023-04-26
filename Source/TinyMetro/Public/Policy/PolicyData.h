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
	FPolicyData() = default;
	FPolicyData(int ServiceCostLevel, bool PrioritySeat, bool HasCCTV, bool HasElevator, bool HasBicycle, bool HasTransfer) :
		ServiceCostLevel(ServiceCostLevel),
		PrioritySeat(PrioritySeat),
		HasCCTV(HasCCTV),
		HasElevator(HasElevator),
		HasBicycle(HasBicycle),
		HasTransfer(HasTransfer) { }


	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int ServiceCostLevel;      // 서비스 비용 단계
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool PrioritySeat;  // 교통약자 좌석 배치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasCCTV;          // CCTV 설치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasElevator;      // 엘레베이터 설치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBicycle;       // 자전거 탑승 허가
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasTransfer;      // 환승 시스템

};
