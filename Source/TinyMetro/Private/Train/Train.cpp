// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"
#include "Train/SubtrainAiController.h"
#include "Station/Station.h"
#include "Components/BoxComponent.h"

void ATrain::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Train"));
}

ATrain::ATrain() {
	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	OverlapVolume->InitBoxExtent(FVector(10,20,30));
	OverlapVolume->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ATrain::OnOverlapBegin, FName("OnOverlapBegin"));;
	OverlapVolume->SetupAttachment(RootComponent);
	//SetRootComponent(OverlapVolume);
}

void ATrain::BeginPlay() {
	Super::BeginPlay();
	TotalTravel = 0.0f;
	ParentAiControllerRef = Cast<ATrainAiController>(GetController());

	if (IsValid(ParentAiControllerRef)) {
		UE_LOG(LogTemp, Log, TEXT("Success"));
	}

}

void ATrain::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AStation::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Overlap"));

	}
	
}

//void ATrain::Tick(float DeltaTime) { }

FVector ATrain::GetNextTrainPosition() {
	return FVector();
}

void ATrain::SetSubtrain(ASubtrain* T) {
	Cast<ASubtrainAiController>(T->GetController())->SetTargetTrain(this);
	Subtrains.Add(T);
}

