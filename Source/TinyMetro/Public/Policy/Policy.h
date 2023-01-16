// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PolicyData.h"
#include "Policy.generated.h"

UCLASS()
class TINYMETRO_API APolicy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APolicy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public : 
	UPROPERTY(BlueprintReadWrite)
		FPolicyData PolicyData;
	

public :
	UFUNCTION(BlueprintCallable, Category = "Policy")
		void SetPolicyData(FPolicyData _policyData);
	UFUNCTION(BlueprintCallable, Category = "Policy")
		FPolicyData GetPolicyData();

	// TODO: HUD 에서 버튼 누르면 바로 반영되게 Set으로 각각 설정

public :

public:
	void InitPolicy();
};
