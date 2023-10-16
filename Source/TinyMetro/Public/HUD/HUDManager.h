#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Texture.h"
#include "PolyMetroWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUDEnums.h"
#include "HUDStructs.h"
#include "Engine/TextRenderActor.h"  // If you're working with TextRender components
#include "Internationalization/Text.h"  // For FText

#include "HUDManager.generated.h"

UCLASS(Blueprintable)
class TINYMETRO_API AHUDManager : public AHUD
{
    GENERATED_BODY()

public:
    AHUDManager();

    UFUNCTION(BlueprintCallable)
    void ChangeLanguage(ELanguage NewLanguage);


protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EHUDText, FLanguagePair> IntegratedTextTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EHUDImage, UTexture*> TextureTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ELanguage CurrentLanguage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ETextSize CurrentTextSize;
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<ETextType, int32> TextSizeMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UFont* KoreanFont;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSlateFontInfo KoreanFontInfo;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FSlateFontInfo EnglishFontInfo;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UFont* EnglishFont;
public:
    //Widgets - Maps with string
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, UPolyMetroWidget*> Widgets;

    UFUNCTION(BlueprintCallable)
    void UpdateWidgets();

    UFUNCTION(BlueprintCallable)
    void AssignWidget(FString Name, UPolyMetroWidget* Widget);

    UFUNCTION(BlueprintCallable)
    UPolyMetroWidget* GetWidget(FString Name);

    UFUNCTION(BlueprintCallable)
    FString GetTextByEnum(EHUDText TextName);

    UFUNCTION(BlueprintCallable)
    UTexture* GetImageByEnum(EHUDImage ImageEnum);

    UFUNCTION(BlueprintCallable)
    void SetTextSizeMap();

    UFUNCTION(BlueprintCallable)
    UFont* GetFont();

    UFUNCTION(BlueprintCallable)
    FSlateFontInfo GetFontInfo();

    UFUNCTION(BlueprintCallable)
    int32 GetTextSizeByType(ETextType TextType);

private:
    void SetImageTable();
    void LoadTextureFromFile(const FString& TexturePath);

    EHUDImage EHUDImageContain(const FString& TextureName);
    void SetIntegradedTextTable();
    void AddTextTable(EHUDText TextEnum, FString Korean, FString English);
};