// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/TMSaveManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATMSaveManager::ATMSaveManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATMSaveManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATMSaveManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATMSaveManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveAllActor();
}

void ATMSaveManager::SaveAllActor()
{
	SaveTask.Broadcast();
}

bool ATMSaveManager::Save(USaveGame* SaveGame, SaveActorType& SaveActor, int32 id)
{
	if (UGameplayStatics::SaveGameToSlot(SaveGame, MakeFileName(SaveActor, id), 0))
	{
		return true;
	}
	else {
		return false;
	}
}

USaveGame* ATMSaveManager::Load(SaveActorType& SaveActor, int32 id)
{
	USaveGame* SaveData = Cast<USaveGame>(UGameplayStatics::LoadGameFromSlot(MakeFileName(SaveActor, id), 0));

	return SaveData;
}

FString ATMSaveManager::MakeFileName(SaveActorType& ActorType, int32 id)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("SaveActorType"), true);

	if (!EnumPtr) return FString("Invalid");

	FString fileName = EnumPtr->GetNameStringByIndex((int32)ActorType);

	if (id == -1)
	{
		return fileName;
	}
	else
	{
		return fileName + TEXT("_") + FString::FromInt(id);

	}
}



