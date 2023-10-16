// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HUDManager.h"

#include "AssetRegistryModule.h"


AHUDManager::AHUDManager()
{
	CurrentLanguage = ELanguage::Korean;
    CurrentTextSize = ETextSize::M;
    SetTextSizeMap();
    SetIntegradedTextTable();
}

void AHUDManager::ChangeLanguage(ELanguage NewLanguage)
{
    CurrentLanguage = NewLanguage;
    // Update your HUD widgets to reflect the new language
    // You can implement this based on your HUD widget design
}


void AHUDManager::SetImageTable()
{
    // To Menu
    LoadTextureFromFile("/Game/UI/HomeWindow/Assets/");

}

void AHUDManager::AssignWidget(FString Name, UPolyMetroWidget* Widget)
{
    Widgets.Add(Name, Widget);
}

UPolyMetroWidget* AHUDManager::GetWidget(FString Name)
{
    UPolyMetroWidget* targetWidget = *Widgets.Find(Name);
    return targetWidget;
}

FString AHUDManager::GetTextByEnum(EHUDText Name)
{
    if (IntegratedTextTable.Find(Name) == nullptr) return "NULL";
    if (CurrentLanguage == ELanguage::Korean) return *IntegratedTextTable[Name].Korean;
    return *IntegratedTextTable[Name].English;
}

void AHUDManager::LoadTextureFromFile(const FString& TexturePath)
{
    // Use the AssetRegistryModule to load the texture by path
    UTexture* LoadedTexture = nullptr;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().GetAssetsByPath(*TexturePath, AssetData);

    // Debugging: Log the AssetData
    for (const FAssetData& Data : AssetData)
    {
        UE_LOG(LogTemp, Warning, TEXT("Found asset: %s"), *Data.AssetName.ToString());
    }

    if (AssetData.Num() > 0)
    {
        for (int32 i = 0; i < AssetData.Num(); i++)
        {
            FString AssetRefPath = AssetData[i].ObjectPath.ToString();
            FStringAssetReference AssetRef(AssetRefPath);
            LoadedTexture = Cast<UTexture>(AssetRef.TryLoad());

            if (LoadedTexture)
            {
                FString LoadedTextureName = *LoadedTexture->GetName();

                EHUDImage matchedEnum = EHUDImageContain(*LoadedTextureName);
                UE_LOG(LogTemp, Error, TEXT("GGGGG:: %s"), *LoadedTextureName);

                if (matchedEnum != EHUDImage::Invalid)
                {
                    UE_LOG(LogTemp, Error, TEXT("GGGGG:: Matched %s"), *LoadedTextureName);
                    TextureTable.Add(matchedEnum, LoadedTexture);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("GGGGG:: UNMatchessd %s"), *LoadedTextureName);

                }
            }

        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GGGGG::No assets found for path: %s"), *TexturePath);
    }    
}

EHUDImage AHUDManager::EHUDImageContain(const FString& TextureName)
{
    FString targetString = "EHUDImage::";
    targetString.Append(TextureName);
    // Check if TextureName exists in EHUDImage enum
    for (int32 i = 0; i < static_cast<int32>(EHUDImage::Invalid); i++)
    {
        FString EnumValueName = UEnum::GetValueAsString<EHUDImage>(static_cast<EHUDImage>(i));
//        UE_LOG(LogTemp, Error, TEXT("GGGGG:: EnumValueName %s"), *EnumValueName);
 
        if (EnumValueName.Equals(targetString, ESearchCase::CaseSensitive))
        {
            UE_LOG(LogTemp, Error, TEXT("GGGGG:: Enum Matched %s"), *EnumValueName);
//            return static_cast<EHUDImage>(i);
        }
    }
    return EHUDImage::Invalid;
}

UTexture* AHUDManager::GetImageByEnum(EHUDImage ImageEnum)
{
    if (TextureTable.Find(ImageEnum) == nullptr) return nullptr;
    return *TextureTable.Find(ImageEnum);
}

void AHUDManager::SetTextSizeMap()
{
    int32 TitleSize = 36;
    int32 BasicSize = 27;
    int32 ExplainationSize = 18;

    switch (CurrentTextSize)
    {
    case ETextSize::S:
        TitleSize -= 4;
        BasicSize -= 4;
        ExplainationSize -= 4;
        break;
    case ETextSize::M:
        break;
    case ETextSize::L:
        TitleSize += 4;
        BasicSize += 4;
        ExplainationSize += 4;
        break;
    default:
        break;
    }
    TMap<ETextType, int32> textSizeMap;
    TextSizeMap.Empty();
    textSizeMap.Add(ETextType::Title, TitleSize);
    textSizeMap.Add(ETextType::Basic, BasicSize);
    textSizeMap.Add(ETextType::ExplainationText, ExplainationSize);

    TextSizeMap = textSizeMap;
}

void AHUDManager::UpdateWidgets()
{
    for (auto& WidgetPair : Widgets)
    {
        UPolyMetroWidget* Widget = WidgetPair.Value;
        if (Widget)
        {
//            Widget->UpdateTextComponents(this);
//            Widget->UpdateImageComponents(this);
            Widget->UpdateWidgets(this);
        }
    }
}

UFont* AHUDManager::GetFont()
{
    switch (CurrentLanguage)
    {
    case ELanguage::English:
        return EnglishFont;
        break;
    case ELanguage::Korean:
        return KoreanFont;
        break;
    default:
        return KoreanFont;
        break;
    }
}

int32 AHUDManager::GetTextSizeByType(ETextType TextType)
{
    return *TextSizeMap.Find(TextType);
}

FSlateFontInfo AHUDManager::GetFontInfo()
{
    switch (CurrentLanguage)
    {
    case ELanguage::English:
        return EnglishFontInfo;
        break;
    case ELanguage::Korean:
        return KoreanFontInfo;
        break;
    default:
        return KoreanFontInfo;
        break;
    }
}

void AHUDManager::SetIntegradedTextTable()
{
    IntegratedTextTable.Empty();
    AddTextTable(EHUDText::None, TEXT("NONE"), TEXT("NONE"));

    //Common Terms
    AddTextTable(EHUDText::Station, TEXT("역"), TEXT("Station"));
    AddTextTable(EHUDText::Shop, TEXT("상점"), TEXT("Shop"));
    AddTextTable(EHUDText::Bank, TEXT("은행"), TEXT("Bank"));
    AddTextTable(EHUDText::Summary, TEXT("요약"), TEXT("Summary"));
    AddTextTable(EHUDText::Lane, TEXT("레인"), TEXT("Lane"));
    AddTextTable(EHUDText::ShowTotal, TEXT("총 표시"), TEXT("Show Total"));
    AddTextTable(EHUDText::GoBack, TEXT("뒤로 가기"), TEXT("Go Back"));
    AddTextTable(EHUDText::Event, TEXT("이벤트"), TEXT("Event"));
    AddTextTable(EHUDText::Train, TEXT("열차"), TEXT("Train"));
    AddTextTable(EHUDText::Subtrain, TEXT("객차"), TEXT("Subtrain"));
    AddTextTable(EHUDText::Tunnel, TEXT("터널"), TEXT("Tunnel"));
    AddTextTable(EHUDText::Bridge, TEXT("다리"), TEXT("Bridge"));
    AddTextTable(EHUDText::Purchase, TEXT("구매"), TEXT("Purchase"));
    AddTextTable(EHUDText::Upgrade, TEXT("업그레이드"), TEXT("Upgrade"));

        //ToMenu
    AddTextTable(EHUDText::ToMenu_ExitText, TEXT("게임 종료"), TEXT("Exit Game"));
    AddTextTable(EHUDText::ToMenu_GoBackText, TEXT("뒤로 가기"), TEXT("Go Back"));
    AddTextTable(EHUDText::ToMenu_StatisticsText, TEXT("통계"), TEXT("Statistics"));
    AddTextTable(EHUDText::ToMenu_Tutorial, TEXT("튜토리얼"), TEXT("Tutorial"));
    AddTextTable(EHUDText::ToMenu_SettingText, TEXT("설정"), TEXT("Settings"));
        //Setting
    AddTextTable(EHUDText::Setting_MaterVolume, TEXT("마스터 볼륨"), TEXT("Master Volume"));
    AddTextTable(EHUDText::Setting_Music, TEXT("음악"), TEXT("Music"));
    AddTextTable(EHUDText::Setting_SoundEffect, TEXT("효과음"), TEXT("Sound Effect"));
    AddTextTable(EHUDText::Setting_CameraSpeed, TEXT("카메라 속도"), TEXT("Camera Speed"));
    AddTextTable(EHUDText::Setting_ChangeSkyByTime, TEXT("시간에 따른 하늘 변화"), TEXT("Change Sky by Time"));
    AddTextTable(EHUDText::Setting_SkySlider, TEXT("하늘 슬라이더"), TEXT("Sky Slider"));
    AddTextTable(EHUDText::Setting_Language, TEXT("언어"), TEXT("Language"));
    AddTextTable(EHUDText::Setting_Language_Korean, TEXT("한국어"), TEXT("Korean"));
    AddTextTable(EHUDText::Setting_Language_English, TEXT("영어(English)"), TEXT("English"));

        // Summary
    AddTextTable(EHUDText::Statistics_Summary_PassengerNum_Total, TEXT("총 승객 수"), TEXT("Total Passengers"));
    AddTextTable(EHUDText::Statistics_Summary_PassengerNum_Weekly, TEXT("주간 승객 수"), TEXT("Weekly Passengers"));
    AddTextTable(EHUDText::Statistics_Summary_Profit_Total, TEXT("총 수익"), TEXT("Total Profit"));
    AddTextTable(EHUDText::Statistics_Summary_Profit_Weekly, TEXT("주간 수익"), TEXT("Weekly Profit"));
    AddTextTable(EHUDText::Statistics_Summary_Expense_Total, TEXT("총 비용"), TEXT("Total Expenses"));
    AddTextTable(EHUDText::Statistics_Summary_Expense_Weekly, TEXT("주간 비용"), TEXT("Weekly Expenses"));
    AddTextTable(EHUDText::Statistics_Summary_AvgComplain, TEXT("평균 불만도"), TEXT("Average Complaints"));
    AddTextTable(EHUDText::Statistics_Summary_OperatingStationNum, TEXT("운영 역"), TEXT("Operating Stations"));
    AddTextTable(EHUDText::Statistics_Summary_UpgradedStationNum, TEXT("업그레이드된 역"), TEXT("Upgraded Stations"));

        // Lane
    AddTextTable(EHUDText::Statistics_Lanes_ConstructedLaneTotalNum, TEXT("건설된 레인"), TEXT("Total Constructed Lanes"));
    AddTextTable(EHUDText::Statistics_Lanes_ModificationDeleteNum, TEXT("노선 수정 및 삭제 횟수"), TEXT("Modified / Deleted Lanes"));
    AddTextTable(EHUDText::Statistics_Lane_UsingBridgeNum, TEXT("이용중인 다리"), TEXT("Using Bridges"));
    AddTextTable(EHUDText::Statistics_Lane_UsingTunnelNum, TEXT("이용중인 터널"), TEXT("Using Tunnels"));
    AddTextTable(EHUDText::Statistics_Lane_TransferStationNum, TEXT("환승 역"), TEXT("Transfer Stations"));
    AddTextTable(EHUDText::Statistics_Lane_OperatingStationNum, TEXT("운영 역"), TEXT("Operating Stations"));
    AddTextTable(EHUDText::Statistics_Lane_ModificationNum, TEXT("축소 연장 횟수"), TEXT("Extend/Reduction Count"));
    AddTextTable(EHUDText::Statistics_Lane_AvgComplainNum, TEXT("평균 불만도"), TEXT("Average Complaints"));
    AddTextTable(EHUDText::Statistics_Lane_OperatingTrainNum, TEXT("운영 중인 열차"), TEXT("Operating Trains"));
    AddTextTable(EHUDText::Statistics_Lane_PassengerNum_Total, TEXT("총 승객"), TEXT("Total Passengers"));
    AddTextTable(EHUDText::Statistics_Lane_ProfitNum_Total, TEXT("총 수익"), TEXT("Total Profit"));
    AddTextTable(EHUDText::Statistics_Lane_ProfitNum_Weekly, TEXT("주간 수익"), TEXT("Weekly Profit"));
    AddTextTable(EHUDText::Statistics_Lane_PassengerNum_Weekly, TEXT("주간 승객 수"), TEXT("Weekly Passengers"));
    
    // StationInfo
    AddTextTable(EHUDText::StationInfo_Complain, TEXT("불만도"), TEXT("Complain"));
    AddTextTable(EHUDText::StationInfo_AvgWaiting, TEXT("평균 대기 시간"), TEXT("Average Waiting Time"));
    AddTextTable(EHUDText::StationInfo_Profit_Total, TEXT("총 수익"), TEXT("Total Profit"));
    AddTextTable(EHUDText::StationInfo_Profit_Weekly, TEXT("주간 수익"), TEXT("Weekly Profit"));
    AddTextTable(EHUDText::StationInfo_Passenger_Total, TEXT("총 승객 수"), TEXT("Total Passengers"));
    AddTextTable(EHUDText::StationInfo_Passenger_Weekly, TEXT("주간 승객 수"), TEXT("Weekly Passengers"));
    AddTextTable(EHUDText::StationInfo_Transfer_Total, TEXT("총 환승 수"), TEXT("Total Transfers"));
    AddTextTable(EHUDText::StationInfo_Transfer_Weekly, TEXT("주간 환승 수"), TEXT("Weekly Transfers"));

    // Train Info
    AddTextTable(EHUDText::TrainInfo_CarNum, TEXT("차량 수"), TEXT("Number of Cars"));
    AddTextTable(EHUDText::TrainInfo_Passenger_Total, TEXT("총 승객 수"), TEXT("Total Passengers"));
    AddTextTable(EHUDText::TrainInfo_Passenger_Weekly, TEXT("주간 승객 수"), TEXT("Weekly Passengers"));

    // Shop - uses only common Terms

    // Bank
    AddTextTable(EHUDText::Bank_Loan, TEXT("대출"), TEXT("Loan"));
    AddTextTable(EHUDText::Bank_PayBack, TEXT("상환"), TEXT("Payback"));
    AddTextTable(EHUDText::Bank_Repay, TEXT("상환"), TEXT("Repay"));
    AddTextTable(EHUDText::Bank_Remainder, TEXT("잔액"), TEXT("Remainder"));

    //Policy
    AddTextTable(EHUDText::Policy_ServiceCost, TEXT("서비스 비용"), TEXT("Service Cost"));
    AddTextTable(EHUDText::Policy_ServiceCost_Explaination, TEXT("서비스 비용 설명"), TEXT("Service Cost Explanation"));
    AddTextTable(EHUDText::Policy_PrioritySeat, TEXT("우선좌석"), TEXT("Priority Seat"));
    AddTextTable(EHUDText::Policy_PrioritySeat_Explaination, TEXT("우선좌석 설명"), TEXT("Priority Seat Explanation"));
    AddTextTable(EHUDText::Policy_CCTV, TEXT("CCTV"), TEXT("CCTV"));
    AddTextTable(EHUDText::Policy_CCTV_Explaination, TEXT("CCTV 설명"), TEXT("CCTV Explanation"));
    AddTextTable(EHUDText::Policy_Elevator, TEXT("엘리베이터"), TEXT("Elevator"));
    AddTextTable(EHUDText::Policy_Elevator_Explaination, TEXT("엘리베이터 설명"), TEXT("Elevator Explanation"));
    AddTextTable(EHUDText::Policy_BicyclePermission, TEXT("자전거 허용"), TEXT("Bicycle Permission"));
    AddTextTable(EHUDText::Policy_BicyclePermission_Explaination, TEXT("자전거 허용 설명"), TEXT("Bicycle Permission Explanation"));
    AddTextTable(EHUDText::Policy_TransferSystem, TEXT("환승 시스템"), TEXT("Transfer System"));
    AddTextTable(EHUDText::Policy_TransferSystem_Explaination, TEXT("환승 시스템 설명"), TEXT("Transfer System Explanation"));
    AddTextTable(EHUDText::Policy_On, TEXT("켜기"), TEXT("On"));
    AddTextTable(EHUDText::Policy_Off, TEXT("끄기"), TEXT("Off"));

    //Event
    
    //StartMenu
    AddTextTable(EHUDText::StartMenu_StartNew, TEXT("새 게임 시작"), TEXT("Start New Game"));
    AddTextTable(EHUDText::StartMenu_Continue, TEXT("이어하기"), TEXT("Continue"));
    AddTextTable(EHUDText::StartMenu_Option, TEXT("옵션"), TEXT("Options"));
    AddTextTable(EHUDText::StartMenu_Exit, TEXT("종료"), TEXT("Exit"));
}

void AHUDManager::AddTextTable(EHUDText TextEnum, FString Korean, FString English)
{
    FLanguagePair Pair;
    Pair.Korean = Korean;
    Pair.English = English;

    IntegratedTextTable.Add(TextEnum, Pair);
}
