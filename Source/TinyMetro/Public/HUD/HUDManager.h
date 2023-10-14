#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Texture.h"
#include "PolyMetroWidget.h"
#include "Blueprint/UserWidget.h"
#include "HUDEnums.h"
#include "HUDStructs.h"

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

public:
    //Widgets - Maps with string
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, UPolyMetroWidget*> Widgets;

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

private:
    void SetImageTable();
    void LoadTextureFromFile(const FString& TexturePath);

    EHUDImage EHUDImageContain(const FString& TextureName);
};