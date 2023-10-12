// Fill out your copyright notice in the Description page of Project Settings.

#include "HUD/HUDManager.h"

#include "AssetRegistryModule.h"


AHUDManager::AHUDManager()
{
	CurrentLanguage = ELanguage::Korean;
    SetStringTable();
    SetImageTable();
}

void AHUDManager::ChangeLanguage(ELanguage NewLanguage)
{
    CurrentLanguage = NewLanguage;
    // Update your HUD widgets to reflect the new language
    // You can implement this based on your HUD widget design
}

void AHUDManager::SetStringTable()
{
    StringTable.Add("ToMenu_ExitText", "Exit");
}

void AHUDManager::SetImageTable()
{
    ImageTable.Add("ToMenu_ExitIcon", LoadTextureFromFile("Texture2D'/Game/UI/HomeWindow/Assets/ExitIcon.ExitIcon'"));
}


FString AHUDManager::GetText(FString BigSection, FString SmallSection)
{
    if (StringTable.Contains(BigSection))
    {
        return StringTable[BigSection];
    }
    return FString(); // Return an empty string if the section is not found
}

void AHUDManager::AssignWidget(FString Name, UPolyMetroWidget* Widget)
{
    Widgets.Add(Name, Widget);
}

UPolyMetroWidget* AHUDManager::GetWidget(FString Name)
{
    UPolyMetroWidget* targetWidget = *Widgets.Find(Name);
//    if(targetWidget == nullptr)
    return targetWidget;
}

FString AHUDManager::GetTextByName(FString Name)
{
    return *StringTable.Find(Name);
}

UTexture* AHUDManager::GetImageByName(FString Name)
{
    return *ImageTable.Find(Name);
}

UTexture* AHUDManager::LoadTextureFromFile(const FString& TexturePath)
{
    // Use the AssetRegistryModule to load the texture by path
    UTexture* LoadedTexture = nullptr;

    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

    TArray<FAssetData> AssetData;
    AssetRegistryModule.Get().GetAssetsByPath(*TexturePath, AssetData);

    if (AssetData.Num() > 0)
    {
        // You can load the first asset found
        FStringAssetReference AssetRef(AssetData[0].ObjectPath.ToString());
        LoadedTexture = Cast<UTexture>(AssetRef.TryLoad());
    }

    return LoadedTexture;
}