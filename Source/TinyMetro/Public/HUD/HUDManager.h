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

UCLASS(Blueprintable)
class TINYMETRO_API AHUDManager : public AHUD
{
    GENERATED_BODY()

public:
    AHUDManager();

    UFUNCTION(BlueprintCallable, Category = "Localization")
    void ChangeLanguage(ELanguage NewLanguage);

    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString GetText(FString BigSection, FString SmallSection);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    TMap<FString, FString> StringTable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Localization")
    TMap<FString, UTexture*> ImageTable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Localization")
    ELanguage CurrentLanguage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Localization")
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
    FString GetTextByName(FString Name);

    UFUNCTION(BlueprintCallable)
    UTexture* GetImageByName(FString Name);

private:
    void SetImageTable();
    void SetStringTable();
    UTexture* LoadTextureFromFile(const FString& TexturePath);

};
