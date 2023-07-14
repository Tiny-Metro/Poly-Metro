// Fill out your copyright notice in the Description page of Project Settings.


#include "Event/TinyMetroEventManager.h"
#include <Kismet/GameplayStatics.h>
#include "GameModes/TinyMetroGameModeBase.h"

// Sets default values
ATinyMetroEventManager::ATinyMetroEventManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATinyMetroEventManager::BeginPlay()
{
	Super::BeginPlay();
	GameModeRef = Cast<ATinyMetroGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));


	GameModeRef->GetTimer()->DailyTask.AddDynamic(this, &ATinyMetroEventManager::DailyTask);
}

void ATinyMetroEventManager::DailyTask() {
	UE_LOG(LogTemp, Log, TEXT("EventManager::DailyTask"));

	// Update event counter
	EventPeriod--;
	if (EventPeriod >= 0) Day++;

	SimulateProbability();
}

void ATinyMetroEventManager::SimulateProbability() {
	// Occur Event
	float EventProbability = (Day > (uint32)EventOccurProbability.Num()) ? 1.0 : EventOccurProbability[Day];
	//float EventProbability = 1.0f;
	if (EventProbability > FMath::RandRange(0.0, 1.0)) {
		UpdateEventWeight();
		OccurEvent();
		Day = 0;
	}
}

void ATinyMetroEventManager::OccurEvent() {
	UE_LOG(LogTemp, Log, TEXT("EventManager::OccurEvent"));
	EventOccurTask.Broadcast();
}

void ATinyMetroEventManager::UpdateEventWeight() {

}

// Called every frame
void ATinyMetroEventManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

