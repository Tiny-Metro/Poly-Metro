// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveSystem/SaveActorType.h"
#include "TMSaveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSaveTask);

UCLASS()
class TINYMETRO_API ATMSaveManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATMSaveManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UPROPERTY(BlueprintAssignable)
	FSaveTask SaveTask;

	UPROPERTY()
	class ATinyMetroGameModeBase* GameModeRef;

	void SaveAllActor();

	//Save, Load Function
	bool Save(class USaveGame* SaveGame, SaveActorType& SaveActor, int32 id = -1);
	class USaveGame* Load(SaveActorType& SaveActor, int32 id = -1);

	FString MakeFileName(SaveActorType& ActorType, int32 id = -1);

};
