// Fill out your copyright notice in the Description page of Project Settings.


#include "Train/Train.h"
#include "Train/SubtrainAiController.h"
#include "Station/Station.h"
#include "Components/BoxComponent.h"
#include "GameModes/GameModeBaseSeoul.h"
#include <Engine/AssetManager.h>

void ATrain::Test() {
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Black,
		TEXT("Test() : Train"));
}

ATrain::ATrain() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TrainMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'")
	);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMaterial(
		TEXT("Material'/Game/Resource/Material/M_Light.M_Light'")
	);
	TrainMaterial.AddUnique(DefaultMaterial.Object->GetMaterial());

	OverlapVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	OverlapVolume->InitBoxExtent(FVector(10,20,30));
	OverlapVolume->OnComponentBeginOverlap.__Internal_AddDynamic(this, &ATrain::OnOverlapBegin, FName("OnOverlapBegin"));;
	OverlapVolume->SetupAttachment(RootComponent);

	TrainMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Train Mesh"));
	TrainMeshComponent->SetWorldScale3D(FVector(2.5f, 1.0f, 1.0f));
	TrainMeshComponent->SetStaticMesh(TrainMesh.Object);
	TrainMeshComponent->GetStaticMesh()->SetMaterial(0, DefaultMaterial.Object);
	TrainMeshComponent->SetupAttachment(RootComponent);
}

void ATrain::BeginPlay() {
	Super::BeginPlay();
	TotalTravel = 0.0f;
	ParentAiControllerRef = Cast<ATrainAiController>(GetController());

	TrainMaterialPath = Cast<AGameModeBaseSeoul>(GetWorld()->GetAuthGameMode())->GetTrainMaterialPath();
	auto& AssetLoader = UAssetManager::GetStreamableManager();
	AssetLoader.RequestAsyncLoad(
		TrainMaterialPath,
		FStreamableDelegate::CreateUObject(this, &ATrain::TrainMaterialDeferred)
	); 
	UE_LOG(LogTemp, Log, TEXT("TUM : %s"), *TrainUpgradeMesh.ToString());
	AssetLoader.RequestAsyncLoad(
		TrainUpgradeMesh,
		FStreamableDelegate::CreateUObject(this, &ATrain::TrainMeshDeferred)
	);
	

	if (IsValid(ParentAiControllerRef)) {
		UE_LOG(LogTemp, Log, TEXT("Success"));
	}

}

void ATrain::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA(AStation::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, TEXT("Overlap"));

	}
	
}

void ATrain::TrainMaterialDeferred() {
	for (auto i : TrainMaterialPath) {
		TAssetPtr<UMaterial> tmp(i);
		TrainMaterial.AddUnique(tmp.Get());
	}
}

void ATrain::TrainMeshDeferred() {
	TrainUpgradeMesh = Cast<UStaticMesh>(TrainUpgradeMeshPath.ResolveObject());
}


//void ATrain::Tick(float DeltaTime) { }

FVector ATrain::GetNextTrainPosition() {
	return FVector();
}

bool ATrain::SetTrainMaterial(int32 LaneNumber) {
	if (!TrainMaterial.IsValidIndex(LaneNumber)) {
		return false;
	}
	TrainMeshComponent->GetStaticMesh()->SetMaterial(0, TrainMaterial[LaneNumber]);
	return true;
}

void ATrain::Upgrade() {
	// Change mesh
	TrainMeshComponent->SetStaticMesh(TrainUpgradeMesh);
	// Set flag
	IsUpgrade = true;
}

void ATrain::SetSubtrain(ASubtrain* T) {
	Cast<ASubtrainAiController>(T->GetController())->SetTargetTrain(this);
	Subtrains.Add(T);
}

