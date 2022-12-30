// Fill out your copyright notice in the Description page of Project Settings.


#include "ComponentTestActor.h"

// Sets default values
AComponentTestActor::AComponentTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Points init
	meshPoints.Add(FVector3d(100, 0, 0));
	meshPoints.Add(FVector3d(-100, 0, 0));
	meshPoints.Add(FVector3d(0, 100, 0));
	meshPoints.Add(FVector3d(0, -100, 0));
	meshPoints.Add(FVector3d(66, 66, 0));
	meshPoints.Add(FVector3d(-66, 66, 0));
	meshPoints.Add(FVector3d(66, -66, 0));
	meshPoints.Add(FVector3d(-66, -66, 0));

	DefaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(DefaultRoot);
	//defaultRoot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	MyScene = CreateDefaultSubobject<USceneComponent>(TEXT("TestRoot"));
	MyScene->SetupAttachment(RootComponent);
	TArray<UActorComponent*> childs = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	int ChildCount = childs.Num();
	for (auto& i : childs) {
		i->DestroyComponent();
	}

	// Load tile's mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> objGround(
		TEXT("StaticMesh'/Game/Station/Asset/StatonMesh/SM_StationCross_Inner.SM_StationCross_Inner'"));
	MyMesh = objGround.Object;

	int num = 0;

	for (auto& i : meshPoints) {
		FName name = *FString::Printf(TEXT("Tile %i"), num++);
		UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(name);
		mesh->SetWorldScale3D(FVector(0.2, 0.2, 0.2));
		mesh->SetRelativeLocation(i);
		mesh->SetStaticMesh(MyMesh);
		mesh->SetupAttachment(MyScene);
	}
	// Init tiles
	/*if (IsValid(GridManager)) {

		for (auto& i : GridPoints) {
			FTransform transform = UKismetMathLibrary::MakeTransform(i.WorldLocation, GetActorRotation());
			FName name = *FString::Printf(TEXT("Tile %i"), i.Index);
			UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(name);
			mesh->SetRelativeTransform(transform);
			switch (i.GridType) {
			case GridType::Ground:
				mesh->SetStaticMesh(meshGround);
				break;
			case GridType::Water:
				mesh->SetStaticMesh(meshWater);
				break;
			case GridType::Hill:
				mesh->SetStaticMesh(meshHill);
				break;
			}
			mesh->AttachToComponent(this->RootComponent, FAttachmentTransformRules::KeepWorldTransform);
		}
		GridManager->SetGridCellData(GridPoints);
		GridManager->SetGridSize(GridSize);
	}*/
}

// Called when the game starts or when spawned
void AComponentTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AComponentTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

