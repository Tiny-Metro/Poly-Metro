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
    int32 ExplainationSize = 22;

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
    AddTextTable(EHUDText::None, "NONE", "NONE");

    //Common Terms
    AddTextTable(EHUDText::Station, "역", "Station");
    AddTextTable(EHUDText::Shop, "상점", "Shop");
    AddTextTable(EHUDText::Bank, "은행", "Bank");
    AddTextTable(EHUDText::Summary, "요약", "Summary");
    AddTextTable(EHUDText::Lane, "레인", "Lane");
    AddTextTable(EHUDText::ShowTotal, "총 표시", "Show Total");
    AddTextTable(EHUDText::GoBack, "뒤로 가기", "Go Back");
    AddTextTable(EHUDText::Event, "이벤트", "Event");
    AddTextTable(EHUDText::Train, "열차", "Train");
    AddTextTable(EHUDText::Subtrain, "객차", "Subtrain");
    AddTextTable(EHUDText::Tunnel, "터널", "Tunnel");
    AddTextTable(EHUDText::Bridge, "다리", "Bridge");
    AddTextTable(EHUDText::Purchase, "구매", "Purchase");
    AddTextTable(EHUDText::Upgrade, "업그레이드", "Upgrade");

        //ToMenu
    AddTextTable(EHUDText::ToMenu_ExitText, "게임 종료", "Exit Game");
    AddTextTable(EHUDText::ToMenu_GoBackText, "뒤로 가기", "Go Back");
    AddTextTable(EHUDText::ToMenu_StatisticsText, "통계", "Statistics");
    AddTextTable(EHUDText::ToMenu_Tutorial, "튜토리얼", "Tutorial");
    AddTextTable(EHUDText::ToMenu_SettingText, "설정", "Settings");
        //Setting
    AddTextTable(EHUDText::Setting_MaterVolume, "마스터 볼륨", "Master Volume");
    AddTextTable(EHUDText::Setting_Music, "음악", "Music");
    AddTextTable(EHUDText::Setting_SoundEffect, "효과음", "Sound Effect");
    AddTextTable(EHUDText::Setting_CameraSpeed, "카메라 속도", "Camera Speed");
    AddTextTable(EHUDText::Setting_ChangeSkyByTime, "시간에 따른 하늘 변화", "Change Sky by Time");
    AddTextTable(EHUDText::Setting_SkySlider, "하늘 슬라이더", "Sky Slider");
    AddTextTable(EHUDText::Setting_Language, "언어", "Language");
    AddTextTable(EHUDText::Setting_Language_Korean, "한국어", "Korean");
    AddTextTable(EHUDText::Setting_Language_English, "영어(English)", "English");

        // Summary
    AddTextTable(EHUDText::Statistics_Summary_PassengerNum_Total, "총 승객 수", "Total Passengers");
    AddTextTable(EHUDText::Statistics_Summary_PassengerNum_Weekly, "주간 승객 수", "Weekly Passengers");
    AddTextTable(EHUDText::Statistics_Summary_Profit_Total, "총 수익", "Total Profit");
    AddTextTable(EHUDText::Statistics_Summary_Profit_Weekly, "주간 수익", "Weekly Profit");
    AddTextTable(EHUDText::Statistics_Summary_Expense_Total, "총 비용", "Total Expenses");
    AddTextTable(EHUDText::Statistics_Summary_Expense_Weekly, "주간 비용", "Weekly Expenses");
    AddTextTable(EHUDText::Statistics_Summary_AvgComplain, "평균 불만도", "Average Complaints");
    AddTextTable(EHUDText::Statistics_Summary_OperatingStationNum, "운영 역", "Operating Stations");
    AddTextTable(EHUDText::Statistics_Summary_UpgradedStationNum, "업그레이드된 역", "Upgraded Stations");

        // Lane
    AddTextTable(EHUDText::Statistics_Lanes_ConstructedLaneTotalNum, "건설된 레인", "Total Constructed Lanes");
    AddTextTable(EHUDText::Statistics_Lanes_ModificationDeleteNum, "노선 수정 및 삭제 횟수", "Modified and Deleted Lanes");
    AddTextTable(EHUDText::Statistics_Lane_UsingBridgeNum, "이용중인 다리", "Using Bridges");
    AddTextTable(EHUDText::Statistics_Lane_UsingTunnelNum, "이용중인 터널", "Using Tunnels");
    AddTextTable(EHUDText::Statistics_Lane_TransferStationNum, "환승 역", "Transfer Stations");
    AddTextTable(EHUDText::Statistics_Lane_OperatingStationNum, "운영 역", "Operating Stations");
    AddTextTable(EHUDText::Statistics_Lane_ModificationNum, "축소 연장 횟수", "Extend/Reduction Count");
    AddTextTable(EHUDText::Statistics_Lane_AvgComplainNum, "평균 불만도", "Average Complaints");
    AddTextTable(EHUDText::Statistics_Lane_OperatingTrainNum, "운영 중인 열차", "Operating Trains");
    AddTextTable(EHUDText::Statistics_Lane_PassengerNum_Total, "총 승객", "Total Passengers");
    AddTextTable(EHUDText::Statistics_Lane_ProfitNum_Total, "총 수익", "Total Profit");
    AddTextTable(EHUDText::Statistics_Lane_PassengerNum_Weekly, "주간 승객 수", "Weekly Passengers");

    // StationInfo
    AddTextTable(EHUDText::StationInfo_Complain, "불만도", "Complain");
    AddTextTable(EHUDText::StationInfo_AvgWaiting, "평균 대기 시간", "Average Waiting Time");
    AddTextTable(EHUDText::StationInfo_Profit_Total, "총 수익", "Total Profit");
    AddTextTable(EHUDText::StationInfo_Profit_Weekly, "주간 수익", "Weekly Profit");
    AddTextTable(EHUDText::StationInfo_Passenger_Total, "총 승객 수", "Total Passengers");
    AddTextTable(EHUDText::StationInfo_Passenger_Weekly, "주간 승객 수", "Weekly Passengers");
    AddTextTable(EHUDText::StationInfo_Transfer_Total, "총 환승 수", "Total Transfers");
    AddTextTable(EHUDText::StationInfo_Transfer_Weekly, "주간 환승 수", "Weekly Transfers");

    // Train Info
    AddTextTable(EHUDText::TrainInfo_CarNum, "차량 수", "Number of Cars");
    AddTextTable(EHUDText::TrainInfo_Passenger_Total, "총 승객 수", "Total Passengers");
    AddTextTable(EHUDText::TrainInfo_Passenger_Weekly, "주간 승객 수", "Weekly Passengers");

    // Shop - uses only common Terms

    // Bank
    AddTextTable(EHUDText::Bank_Loan, "대출", "Loan");
    AddTextTable(EHUDText::Bank_PayBack, "상환", "Payback");
    AddTextTable(EHUDText::Bank_Repay, "상환", "Repay");
    AddTextTable(EHUDText::Bank_Remainder, "잔액", "Remainder");

    //Policy
    AddTextTable(EHUDText::Policy_ServiceCost, "서비스 비용", "Service Cost");
    AddTextTable(EHUDText::Policy_ServiceCost_Explaination, "서비스 비용 설명", "Service Cost Explanation");
    AddTextTable(EHUDText::Policy_PrioritySeat, "우선좌석", "Priority Seat");
    AddTextTable(EHUDText::Policy_PrioritySeat_Explaination, "우선좌석 설명", "Priority Seat Explanation");
    AddTextTable(EHUDText::Policy_CCTV, "CCTV", "CCTV");
    AddTextTable(EHUDText::Policy_CCTV_Explaination, "CCTV 설명", "CCTV Explanation");
    AddTextTable(EHUDText::Policy_Elevator, "엘리베이터", "Elevator");
    AddTextTable(EHUDText::Policy_Elevator_Explaination, "엘리베이터 설명", "Elevator Explanation");
    AddTextTable(EHUDText::Policy_BicyclePermission, "자전거 허용", "Bicycle Permission");
    AddTextTable(EHUDText::Policy_BicyclePermission_Explaination, "자전거 허용 설명", "Bicycle Permission Explanation");
    AddTextTable(EHUDText::Policy_TransferSystem, "환승 시스템", "Transfer System");
    AddTextTable(EHUDText::Policy_TransferSystem_Explaination, "환승 시스템 설명", "Transfer System Explanation");
    AddTextTable(EHUDText::Policy_On, "켜기", "On");
    AddTextTable(EHUDText::Policy_Off, "끄기", "Off");

    //Event
    
    //StartMenu
    AddTextTable(EHUDText::StartMenu_StartNew, "새 게임 시작", "Start New Game");
    AddTextTable(EHUDText::StartMenu_Continue, "이어하기", "Continue");
    AddTextTable(EHUDText::StartMenu_Option, "옵션", "Options");
    AddTextTable(EHUDText::StartMenu_Exit, "종료", "Exit");
}

void AHUDManager::AddTextTable(EHUDText TextEnum, FString Korean, FString English)
{
    FLanguagePair Pair;
    Pair.Korean = Korean;
    Pair.English = English;

    IntegratedTextTable.Add(TextEnum, Pair);
}
