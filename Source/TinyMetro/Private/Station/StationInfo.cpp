// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/StationInfo.h"

uint32 GetTypeHash(const FStationInfo& StationInfo)
{
	return FCrc::MemCrc32(&StationInfo, sizeof(FStationInfo));
}
