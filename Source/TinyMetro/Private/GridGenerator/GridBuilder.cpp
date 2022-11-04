// Fill out your copyright notice in the Description page of Project Settings.


#include "GridGenerator/GridBuilder.h"
#include "GridGenerator/GridManager.h"
#include "TextReader.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include <Kismet/KismetStringLibrary.h>
#include <Kismet/KismetMathLibrary.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AGridBuilder::AGridBuilder() : GridCountX(0), GridCountY(0), GeneratorIndex(0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	defaultRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRoot"));
	SetRootComponent(defaultRoot);
	TArray<UActorComponent*> childs = GetComponentsByClass(UStaticMeshComponent::StaticClass());
	ChildCount = childs.Num();
	for (auto& i : childs) {
		i->DestroyComponent();
	}

	// Load tile's mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> objGround(
		TEXT("StaticMesh'/Game/GridBuilder/Meshes/SM_GirdCell_Ground.SM_GirdCell_Ground'"));
	meshGround = objGround.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> objWater(
		TEXT("StaticMesh'/Game/GridBuilder/Meshes/SM_GridCell_Water.SM_GridCell_Water'"));
	meshWater = objWater.Object;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> objHill(
		TEXT("StaticMeshVersionPlusOne'/Game/GridBuilder/Meshes/SM_GirdCell_Hill.SM_GirdCell_Hill'"));
	meshHill = objHill.Object;

	// Init GridManager
	GridManager = Cast<AGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGridManager::StaticClass()));

	// Init tiles
	if (IsValid(GridManager)) {
		GridPoints = CalculateGridData(GridManager->GetGridCellSize());
		
		for (auto& i : GridPoints) {
			FTransform transform = UKismetMathLibrary::MakeTransform(i.WorldLocation, GetActorRotation());
			FName name = *FString::Printf(TEXT("Tile %i"), i.Index);
			UPROPERTY()
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
	}

}

FVector AGridBuilder::CalcStartLocation(float gridCellSize) const {
	using Math = UKismetMathLibrary;
	/*
	* [
	* [
	* [[ActorLocation]-[[Y-1]x[Size/2]]X[ActorRightVector]]
	* -
	* [[[X-1]X[Size/2]]X[ActorForwardVector]]
	* ]
	* +
	* [ActorUpVector*1]
	* ]
	* = StartLocation
	*/
	return Math::Add_VectorVector(
		Math::Subtract_VectorVector(
			Math::Subtract_VectorVector(
				GetActorLocation(),
				Math::Multiply_VectorFloat(
					GetActorRightVector(),
					Math::Multiply_FloatFloat(
						GridCountY - 1,
						gridCellSize / 2
					)
				)
			),
			Math::Multiply_VectorFloat(
				GetActorForwardVector(),
				Math::Multiply_FloatFloat(
					GridCountX - 1,
					gridCellSize / 2
				)
			)
		),
		Math::Multiply_VectorFloat(
			GetActorUpVector(),
			1.0f
		)
	);
}

// Called when the game starts or when spawned
void AGridBuilder::BeginPlay()
{
	Super::BeginPlay();
	
}

TArray<FGridCellData> AGridBuilder::GetGridPoints() const {
	return GridPoints;
}

void AGridBuilder::LoadMapData() {
	FString outText;
	LoadedGridType.Empty();
	if (UTextReader::LoadMapText(TEXT("map.txt"), outText)) {
		TArray<FString> GridTypeText;
		TArray<FString> sizeAndType = UKismetStringLibrary::ParseIntoArray(
			outText,
			TEXT("/"));
		FString mapSizeVector = sizeAndType[0];
		FString cellTypeVector = sizeAndType[1];
		TArray<FString> mapSize = UKismetStringLibrary::ParseIntoArray(
			mapSizeVector,
			TEXT(","),
			true);

		GridCountX = FCString::Atoi(*mapSize[0]);
		GridCountY = FCString::Atoi(*mapSize[1]);

		cellTypeVector = cellTypeVector.Replace(
			LINE_TERMINATOR,
			TEXT(""));
		cellTypeVector.ParseIntoArray(
			GridTypeText,
			TEXT(","),
			true
		);

		for (auto& i : GridTypeText) {
			LoadedGridType.Add(FCString::Atoi(*i));
		}
	}
}

TArray<FGridCellData> AGridBuilder::CalculateGridData(float GridCellSize) {
	using Math = UKismetMathLibrary;
	this->LoadMapData();
	StartLocation = CalcStartLocation(GridCellSize);
	TArray<FGridCellData> gridCellData;
	//gridCellData;

	// i = Y, j = X
	// GridIndex = (X * i) + j
	for (int i = 0; i < GridCountY; i++) {
		for (int j = 0; j < GridCountX; j++) {
			FGridCellData tmp;
			int indexOneDim = GridCountX * i + j;
			FVector cellLocation = Math::Add_VectorVector(
				StartLocation,
				Math::Add_VectorVector(
					Math::Multiply_VectorFloat(
						GetActorRightVector(),
						Math::Multiply_IntFloat(
							i,
							GridCellSize
						)
					),
					Math::Multiply_VectorFloat(
						GetActorForwardVector(),
						Math::Multiply_IntFloat(
							j,
							GridCellSize
						)
					)
				)
			);
			tmp.Index = indexOneDim;
			tmp.WorldLocation = cellLocation;
			switch (LoadedGridType[indexOneDim]) {
			case 0:
				tmp.GridType = GridType::Ground;
				break;
			case 1:
				tmp.GridType = GridType::Water;
				break;
			case 2:
				tmp.GridType = GridType::Hill;
				break;
			default:
				tmp.GridType = GridType::Ground;
				break;
			}
			gridCellData.Emplace(tmp);
		}
	}

	return gridCellData;
}


