// Fill out your copyright notice in the Description page of Project Settings.
#include "HUD/PolyMetroWidget.h"
#include "HUD/HUDManager.h"


void UPolyMetroWidget::AssignTextComponent(FTextComponent NewTextComponent)
{
	TextComponents.Add(NewTextComponent);
}
void UPolyMetroWidget::AssignImageComponent(FImageComponent NewImageComponent)
{
	ImageComponents.Add(NewImageComponent);
}
void UPolyMetroWidget::UpdateTextComponents(AHUDManager* hudManager)
{
	for (FTextComponent targetTextComponent : TextComponents)
	{
		FString targetString = hudManager->GetTextByEnum(targetTextComponent.TextEnum);
		FText targetText = FText::FromString(targetString);
		targetTextComponent.TextWidgetREF->SetText(targetText);

		FSlateFontInfo targetFontInfo = hudManager->GetFontInfo();
		int32 textSize = hudManager->GetTextSizeByType(targetTextComponent.TextType);
		targetFontInfo.Size = textSize;

		targetTextComponent.TextWidgetREF->SetFont(targetFontInfo);
	}
}
void UPolyMetroWidget::UpdateImageComponents(AHUDManager* hudManager)
{
	for (FImageComponent targetImage : ImageComponents)
	{
		UTexture* targetTexture = hudManager->GetImageByEnum(targetImage.ImageEnum);
		targetImage.ImageREF->SetBrushFromSoftTexture(targetTexture);
	}
}
void UPolyMetroWidget::UpdateWidgets(AHUDManager* hudManager)
{
	for (UPolyMetroWidget* targetWidget : Widgets)
	{
		targetWidget->UpdateTextComponents(hudManager);
		targetWidget->UpdateImageComponents(hudManager);
		if (targetWidget->Widgets.Num() > 0)
		{
			targetWidget->UpdateWidgets(hudManager);
		}
	}
}