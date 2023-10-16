// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HUDEnums.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class ELanguage : uint8
{
    English,
    Korean
};

UENUM(BlueprintType)
enum class ETextSize : uint8
{
    S,
    M,
    L

};
UENUM(BlueprintType)
enum class ETextType : uint8
{
    Title,
    Basic,
    ExplainationText
};

UENUM(BlueprintType)
enum class EHUDText : uint8
{
    // Reamin Enums
    //  []Statistics - Shop
    //  []Statistics - Bank
    //  []Events

    None,
    //Common Terms
    Station,
    Shop,
    Bank,
    Summary,
    Lane,
    ShowTotal,
    GoBack,
    Event,
    Train,
    Subtrain,
    Tunnel,
    Bridge,
    Purchase,
    Upgrade,

    //ToMenu
    ToMenu_ExitText,
    ToMenu_GoBackText,
    ToMenu_StatisticsText,
    ToMenu_Tutorial,
    ToMenu_SettingText,
    //Setting
    Setting_MaterVolume,
    Setting_Music,
    Setting_SoundEffect,
    Setting_CameraSpeed,
    Setting_ChangeSkyByTime,
    Setting_SkySlider,
    Setting_Language,
    Setting_Language_Korean,
    Setting_Language_English,

    // Summary
    Statistics_Summary_PassengerNum_Total,
    Statistics_Summary_PassengerNum_Weekly,
    Statistics_Summary_Profit_Total,
    Statistics_Summary_Profit_Weekly,
    Statistics_Summary_Expense_Total,
    Statistics_Summary_Expense_Weekly,
    Statistics_Summary_AvgComplain,
    Statistics_Summary_OperatingStationNum,
    Statistics_Summary_UpgradedStationNum,
    // Lane
    Statistics_Lanes_ConstructedLaneTotalNum,
    Statistics_Lanes_ModificationDeleteNum,
    Statistics_Lane_UsingBridgeNum,
    Statistics_Lane_UsingTunnelNum,
    Statistics_Lane_TransferStationNum,
    Statistics_Lane_OperatingStationNum,
    Statistics_Lane_ModificationNum,
    Statistics_Lane_AvgComplainNum,
    Statistics_Lane_OperatingTrainNum,
    Statistics_Lane_PassengerNum_Total,
    Statistics_Lane_ProfitNum_Total,
    Statistics_Lane_PassengerNum_Weekly,
    Statistics_Lane_ProfitNum_Weekly,
    Statistics_Lane_LaneSelection,
    // Shop
    Statistics_Shop_SpendMoney,
    Statistics_Shop_TotalPurchaseCount,
    Statistics_Shop_PurchaseCount,
    Statistics_Shop_PlacedCount,
    Statistics_Shop_MovedCount,
    Statistics_Shop_RecollectedCount,
    Statistics_Shop_UpgradeCount,
    Statistics_Shop_ObjectSelection,
    // Bank
    Statistics_Bank_LoanUseCount,
    Statistics_Bank_TotalRepayment,
    Statistics_Bank_TotalLoanAmount,
    Statistics_Bank_TotalPaidInterest,
  //StationInfo
    StationInfo_Complain,
    StationInfo_AvgWaiting,
    StationInfo_Profit_Total,
    StationInfo_Profit_Weekly,
    StationInfo_Passenger_Total,
    StationInfo_Passenger_Weekly,
    StationInfo_Transfer_Total,
    StationInfo_Transfer_Weekly,

    // Train Info
    TrainInfo_CarNum,
    TrainInfo_Passenger_Total,
    TrainInfo_Passenger_Weekly,

    // Shop - uses only common Terms

    // Bank
    Bank_Loan,
    Bank_PayBack,
    Bank_Repay,
    Bank_Remainder,
    //Policy
    Policy_ServiceCost,
    Policy_ServiceCost_Explaination,
    Policy_PrioritySeat,
    Policy_PrioritySeat_Explaination,
    Policy_CCTV,
    Policy_CCTV_Explaination,
    Policy_Elevator,
    Policy_Elevator_Explaination,
    Policy_BicyclePermission,
    Policy_BicyclePermission_Explaination,
    Policy_TransferSystem,
    Policy_TransferSystem_Explaination,
    Policy_On,
    Policy_Off,
    //Event

    //StartMenu
    StartMenu_StartNew,
    StartMenu_Continue,
    StartMenu_Option,
    StartMenu_Exit
    //
};

UENUM(BlueprintType)
enum class EHUDImage : uint8
{
    Invalid,
    ToMenu_ExitIcon,
    ToMenu_GoBackIcon,
    ToMenu_StatisticsIcon,
    ToMenu_SettingIcon,
    ToMenu_TutorialIcon,
    Station_Circle,
    Station_Triangle,
    Station_Rectangle,
    Station_Cross,
    Station_Rhombus,
    Station_Oval,
    Station_Diamond,
    Station_Pentagon,
    Station_Star,
    Station_Fan,
    Train_Train,
    Train_SubTrain
};