// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HUDManager.h"

#include "AssetRegistryModule.h"


AHUDManager::AHUDManager()
{
	CurrentLanguage = ELanguage::Korean;
    CurrentTextSize = ETextSize::M;
    SetTextSizeMap();
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