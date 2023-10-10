// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TinyMetroEffectSound.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class TINYMETRO_API ASoundManager : public AActor
{
	GENERATED_BODY()
		
protected:
	virtual void BeginPlay() override;
		
public:
	ASoundManager();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void PlaySound(TinyMetroEffectSound SoundType);
	UFUNCTION(BlueprintCallable)
	void PlayRandomBGM();
	UFUNCTION(BlueprintCallable)
	void PlayTitleBGM();

	// Volume getter, setter
	UFUNCTION(BlueprintCallable)
	float GetBackgroundVolume() const;
	UFUNCTION(BlueprintCallable)
	void SetBackgroundVolume(float NewVolume);
	UFUNCTION(BlueprintCallable)
	float GetEffectVolume() const;
	UFUNCTION(BlueprintCallable)
	void SetEffectVolume(float NewVolume);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volume")
	float BackgroundVolume = 1.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Volume")
	float EffectVolume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PolyMetro")
	TObjectPtr<UAudioComponent> TestEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PolyMetro")
	USoundBase* TestSoundBase;
	
	// Effect sounds
	UPROPERTY(VisibleAnywhere, Category = "Effect Sound")
	TMap<TinyMetroEffectSound, FString> EffectSoundObjectPath = {
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::StationSpawn, TEXT("SoundWave'/Game/Sound/SoundEffect/StationSpawn.StationSpawn'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::SpawnTrain, TEXT("SoundWave'/Game/Sound/SoundEffect/SpawnTrain.SpawnTrain'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::SpawnSubtrain, TEXT("SoundWave'/Game/Sound/SoundEffect/SpawnSubtrain.SpawnSubtrain'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::TrainUpgrade, TEXT("SoundWave'/Game/Sound/SoundEffect/TrainUpgrade.TrainUpgrade'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::StationUpgrade, TEXT("SoundWave'/Game/Sound/SoundEffect/StationUpgrade.StationUpgrade'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::OccurEvent, TEXT("SoundWave'/Game/Sound/SoundEffect/OccurEvent.OccurEvent'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::ButtonClick, TEXT("SoundWave'/Game/Sound/SoundEffect/ButtonClick.ButtonClick'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::TabMove, TEXT("SoundWave'/Game/Sound/SoundEffect/TabMove.TabMove'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::Pause, TEXT("SoundWave'/Game/Sound/SoundEffect/Pause.Pause'")),
		TPair<TinyMetroEffectSound, FString>(TinyMetroEffectSound::Unpause, TEXT("SoundWave'/Game/Sound/SoundEffect/UnPause.UnPause'"))
	};
	UPROPERTY()
	TMap<TinyMetroEffectSound, USoundBase*> EffectSoundBase;
	UPROPERTY(VisibleAnywhere, Category = "Effect Sound")
	TMap<TinyMetroEffectSound, TObjectPtr<UAudioComponent>> EffectSound;

	// Background sounds
	UPROPERTY(VisibleAnywhere, Category = "Background Sound")
	TArray<FString> BackgroundSoundObjectPath = {
		TEXT("SoundWave'/Game/Sound/BGM/BGM_01.BGM_01'"),
		TEXT("SoundWave'/Game/Sound/BGM/BGM_02.BGM_02'"),
		TEXT("SoundWave'/Game/Sound/BGM/BGM_03.BGM_03'"),
		TEXT("SoundWave'/Game/Sound/BGM/BGM_04.BGM_04'"),
		TEXT("SoundWave'/Game/Sound/BGM/BGM_05.BGM_05'"),
		TEXT("SoundWave'/Game/Sound/BGM/BGM_06.BGM_06'")
	};
	UPROPERTY(VisibleAnywhere, Category = "Background Sound")
	TArray<USoundBase*> BackgroundSoundBase;
	UPROPERTY(VisibleAnywhere, Category = "Background Sound")
	TArray<TObjectPtr<UAudioComponent>> BackgroundSound;

	// Background sounds (Title)
	UPROPERTY(VisibleAnywhere, Category = "Background Sound")
	FString TitleBackgroundSoundObjectPath = TEXT("SoundWave'/Game/Sound/BGM/BGM_01.BGM_01'");
	UPROPERTY(VisibleAnywhere, Category = "Background Sound")
	USoundBase* TitleBackgroundSoundBase;
	UPROPERTY(VisibleAnywhere, Category = "Background Sound")
	TObjectPtr<UAudioComponent> TitleBackgroundSound;

};
