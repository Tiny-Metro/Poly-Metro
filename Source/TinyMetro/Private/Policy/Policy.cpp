// Fill out your copyright notice in the Description page of Project Settings.


#include "Policy/Policy.h"

// Sets default values
APolicy::APolicy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APolicy::BeginPlay()
{
	Super::BeginPlay();

	InitPolicy();
	
}

// Called every frame
void APolicy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APolicy::SetPolicyData(FPolicyData _policyData) {
	this->PolicyData = _policyData;

	// 서비스 비용 레벨

}

FPolicyData APolicy::GetPolicyData() {

	return PolicyData;
}

void APolicy::InitPolicy() {
}