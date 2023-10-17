// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/TMSaveManager.h"
#include "GameModes/TinyMetroGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

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

	// Add AutoSave
	GameModeRef = Cast<ATinyMetroGameModeBase>(GetWorld()->GetAuthGameMode());
	GameModeRef->GetTimer()->WeeklyTask.AddDynamic(this, &ATMSaveManager::SaveAllActor);
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

void ATMSaveManager::RemoveSaveData(FString LevelName) {
	FString saveDir = UKismetSystemLibrary::GetProjectSavedDirectory();
	saveDir.Append(TEXT("SaveGames"))
		.Append(FGenericPlatformMisc::GetDefaultPathSeparator())
		.Append(LevelName);

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	platformFile.DeleteDirectory(*saveDir);

}

void ATMSaveManager::SaveAllActor()
{
	SaveTask.Broadcast();
}

bool ATMSaveManager::Save(USaveGame* SaveGame, SaveActorType SaveActor, int32 id)
{
	if (UGameplayStatics::SaveGameToSlot(SaveGame, MakeFileName(SaveActor, id), 0))
	{
		return true;
	}
	else {
		return false;
	}
}

USaveGame* ATMSaveManager::Load(SaveActorType SaveActor, int32 id)
{
	USaveGame* SaveData = UGameplayStatics::LoadGameFromSlot(MakeFileName(SaveActor, id), 0);

	return SaveData;
}

FString ATMSaveManager::MakeFileName(SaveActorType ActorType, int32 id)
{
	//Enum to String
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("SaveActorType"), true);

	if (!EnumPtr) return FString("Invalid");

	FString fileName = EnumPtr->GetNameStringByIndex((int32)ActorType);

	FString levelName = UGameplayStatics::GetCurrentLevelName(GetWorld());

	fileName = levelName + TEXT("/") + fileName;

	//Check id
	if (id == -1)
	{
		return fileName;
	}
	else
	{
		return fileName + TEXT("_") + FString::FromInt(id);

	}
}