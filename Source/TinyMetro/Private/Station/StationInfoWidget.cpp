// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationInfoWidget.h"
#include <UMG/Public/Blueprint/WidgetLayoutLibrary.h>

void UStationInfoWidget::NativeConstruct() {
	Super::NativeConstruct();
}

void UStationInfoWidget::ShowWidget_Implementation(AStation* Station) {
	StationRef = Station;
}
