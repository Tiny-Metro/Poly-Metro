// Fill out your copyright notice in the Description page of Project Settings.


#include "Sound/SoundManager.h"
#include "Sound/SoundManagerSaveGame.h"
#include "Components/AudioComponent.h"
#include "SaveSystem/TMSaveManager.h"
#include "Kismet/GameplayStatics.h"

ASoundManager::ASoundManager() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.5f;
	// Construct background sounds
	for (auto& i : BackgroundSoundObjectPath) {
		ConstructorHelpers::FObjectFinder<USoundBase> tmp(*i);
		if (tmp.Succeeded()) BackgroundSoundBase.Emplace(tmp.Object);
	}

	// Construct background sound (Title)
	ConstructorHelpers::FObjectFinder<USoundBase> titleBGM(*TitleBackgroundSoundObjectPath);
	if (titleBGM.Succeeded()) TitleBackgroundSoundBase = titleBGM.Object;
		
	// Construct effect sounds
	for (auto& i : EffectSoundObjectPath) {
		ConstructorHelpers::FObjectFinder<USoundBase> tmp(*i.Value);
		if (tmp.Succeeded()) EffectSoundBase.Emplace(i.Key, tmp.Object);
	}

}

void ASoundManager::BeginPlay() {
	Super::BeginPlay();

	// Soundbase to audio component(Background)
	for (auto& i : BackgroundSoundBase) {
		BackgroundSound.Emplace(UGameplayStatics::CreateSound2D(GetWorld(), i, 1.0f, 1.0f, 0.0f, nullptr, false, false));
	}

	// Soundbase to audio component(Background title)
	TitleBackgroundSound = UGameplayStatics::CreateSound2D(GetWorld(), TitleBackgroundSoundBase, 1.0f, 1.0f, 0.0f, nullptr, false, false);

	// Soundbase to audio component(Effect)
	for (auto& i : EffectSoundBase) {
		EffectSound.Emplace(i.Key, UGameplayStatics::CreateSound2D(GetWorld(), i.Value, 1.0f, 1.0f, 0.0f, nullptr, false, false));
	}

	for (auto& i : BackgroundSound) {
		i->OnAudioFinished.AddDynamic(this, &ASoundManager::PlayRandomBGM);
	}

	FindReferenceClass();
	Load();
	SaveManagerRef->SaveTask.AddDynamic(this, &ASoundManager::Save);

	if (UGameplayStatics::GetCurrentLevelName(GetWorld()) == TEXT("LV_StartMenu")) {
		PlayTitleBGM();
	} else {
		PlayRandomBGM();
	}
}

void ASoundManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void ASoundManager::FindReferenceClass() {
	if (!SaveManagerRef) SaveManagerRef = Cast<ATMSaveManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ATMSaveManager::StaticClass()));
}

void ASoundManager::PlaySound(TinyMetroEffectSound SoundType) {
	if (EffectSound.Contains(SoundType)) {
		EffectSound[SoundType]->Play();
	}
}

void ASoundManager::PlayRandomBGM() {
	for (auto& i : BackgroundSound) {
		if (IsValid(i)) {
			if (i->IsPlaying()) i->Stop();
		}
	}

	int randIdx = FMath::RandRange(0, BackgroundSound.Num() - 1);
	if (BackgroundSound.IsValidIndex(randIdx)) {
		if (IsValid(BackgroundSound[randIdx])) {
			BackgroundSound[randIdx]->Play();
		}
	}
}

void ASoundManager::PlayTitleBGM() {
	if (IsValid(TitleBackgroundSound)) {
		TitleBackgroundSound->Play();
	}
}

float ASoundManager::GetMasterVolume() const {
	return MasterVolume;
}

void ASoundManager::SetMasterVolume(float NewVolume) {
	MasterVolume = NewVolume;
	SetBackgroundVolume(BackgroundVolume);
	SetEffectVolume(EffectVolume);
}

float ASoundManager::GetBackgroundVolume() const {
	return BackgroundVolume;
}

void ASoundManager::SetBackgroundVolume(float NewVolume) {
	BackgroundVolume = NewVolume;
	for (auto& i : BackgroundSound) {
		i->SetVolumeMultiplier(BackgroundVolume * MasterVolume);
	}
}

float ASoundManager::GetEffectVolume() const {
	return EffectVolume;
}

void ASoundManager::SetEffectVolume(float NewVolume) {
	EffectVolume = NewVolume;
	for (auto& i : EffectSound) {
		i.Value->SetVolumeMultiplier(EffectVolume * MasterVolume);
	}
}

void ASoundManager::Save() {
	FindReferenceClass();
	USoundManagerSaveGame* tmp = Cast<USoundManagerSaveGame>(UGameplayStatics::CreateSaveGameObject(USoundManagerSaveGame::StaticClass()));

	tmp->MasterVolume = MasterVolume;
	tmp->BackgroundVolume = BackgroundVolume;
	tmp->EffectVolume = EffectVolume;

	SaveManagerRef->Save(tmp, SaveActorType::SoundManager);
}

void ASoundManager::Load() {
	FindReferenceClass();
	USoundManagerSaveGame* tmp = Cast<USoundManagerSaveGame>(SaveManagerRef->Load(SaveActorType::SoundManager));

	if (!IsValid(tmp)) {
		return;
	}
	
	MasterVolume = tmp->MasterVolume;
	BackgroundVolume = tmp->BackgroundVolume;
	EffectVolume = tmp->EffectVolume;

	SetMasterVolume(MasterVolume);
}
