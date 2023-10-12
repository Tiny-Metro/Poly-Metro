#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Texture.h"
#include "PolyMetroWidget.h"
#include "Blueprint/UserWidget.h"

#include "HUDManager.generated.h"

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
enum class EHUDText : uint8
{
    ToMenu_ExitText,
    ToMenu_GoBackText,
    ToMenu_StatisticsText,
    ToMenu_SettingText,
    None
};

UENUM(BlueprintType)
enum class EHUDImage: uint8
{
    ToMenu_ExitIcon,
    ToMenu_GoBackIcon,
    ToMenu_StatisticsIcon,
    ToMenu_SettingIcon,
    Invalid
};

USTRUCT(BlueprintType)
struct TINYMETRO_API FLanguagePair
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Korean;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
        FString English;
};

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

private:
    void SetImageTable();
    void LoadTextureFromFile(const FString& TexturePath);

    EHUDImage EHUDImageContain(const FString& TextureName);
};