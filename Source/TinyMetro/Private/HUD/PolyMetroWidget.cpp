// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PolyMetroWidget.h"

FString UPolyMetroWidget:: GetTextByName(FString Name)
{
	TMap<FString, FString> map;

	map.Add(("GoBack", "Go Back"));
	map.Add(("Statistics", "Statistics!"));
	map.Add(("Option", "OPTION"));
	map.Add(("Tutorial", "turial"));
	map.Add(("Exit", "Exit"));

	TextBlockMap = map;
	

	FString* targetString = map.Find(Name);

	if (targetString == nullptr)
	{		
		UE_LOG(LogTemp, Warning, TEXT("No Such Text"));
		
		return "ERROR";
	}
	return *targetString;
}