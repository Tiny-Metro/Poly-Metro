// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/GamePlayInfo.h"
#include "PlayerItem.h"
#include "../Shop/ItemType.h"
#include "../Timer/Timer.h"
#include "GameOverState.h"
#include "TinyMetroPlayerState.generated.h"

class ATinyMetroGameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameOverTask);

UCLASS()
class TINYMETRO_API ATinyMetroPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FGamePlayInfo GetPlayInfo();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	// Call by shop
	UFUNCTION(BlueprintCallable)
	bool BuyItem(ItemType Type, int32 Cost, int32 Amount);
	UFUNCTION(BlueprintCallable)
	bool CanUseMoney(int32 Val) const;
	
	UFUNCTION(BlueprintCallable)
	int32 GetMoney();
	UFUNCTION(BlueprintCallable)
	FPlayerItem GetValidItem();
	UFUNCTION(BlueprintCallable)
	FPlayerItem GetUsingItem();

	UFUNCTION()
	int32 GetValidBridgeCount();

	UFUNCTION()
	int32 GetValidTunnelCount();

public:
	UFUNCTION(BlueprintCallable)
	void AddMoney(int32 Amount);
	UFUNCTION(BlueprintCallable)
	void AddIncome(int32 Amount);
	void Test();

	UFUNCTION(BlueprintCallable)
	void AddItem(ItemType Item, int Amount);
	UFUNCTION(BlueprintCallable)
	bool UseLane();
	UFUNCTION(BlueprintCallable)
	bool UseTrain();
	UFUNCTION(BlueprintCallable)
	bool UseSubtrain();
	UFUNCTION(BlueprintCallable)
	bool UseBridge();
	UFUNCTION(BlueprintCallable)
	bool UseTunnel();

	UFUNCTION()
	void Save();
	UFUNCTION()
	void Load();

	UFUNCTION()
	void WeeklyTask();

	UFUNCTION(BlueprintCallable)
	void ResetTutorialProceed();

	UFUNCTION()
	void GameOverByComplain();
	UFUNCTION()
	void GameOverByBankruptcy();
	UPROPERTY(BlueprintAssignable)
	FGameOverTask GameOverTask;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
	int32 Money = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPlayerItem ValidItem;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
	FPlayerItem UsingItem;

	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStationManager* StationManager;
	UPROPERTY(VisibleAnywhere, Category = "Config")
	class AStatisticsManager* StatisticsManagerRef;
	UPROPERTY(VisibleAnywhere, Category = "Config")
	class ATimer* TimerRef;
	UPROPERTY(VisibleAnywhere, Category = "Config")
	class ATMSaveManager* SaveManagerRef;
	UPROPERTY(VisibleAnywhere, Category = "Config")
	class ATinyMetroGameModeBase* GameModeRef;
	UPROPERTY(VisibleAnywhere, Category = "Config")
	class ATinyMetroCamera* CameraRef;

	UPROPERTY(VisibleAnywhere, Category = "GameOver")
	bool BankruptcyWarning = false;
	UPROPERTY(VisibleAnywhere, Category = "GameOver")
	GameOverState GameOverState = GameOverState::Playing;

	// Tutorial finish flags
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsItemTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsLaneTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsStationTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsTrainTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsEventTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsShopTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsLoanTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsInvestmentTutorialFinished = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	bool IsPolicyTutorialFinished = false;
};
