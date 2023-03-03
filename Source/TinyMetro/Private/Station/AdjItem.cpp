// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/AdjItem.h"

uint32 GetTypeHash(const FAdjItem& AdjItem)
{
	return FCrc::MemCrc32(&AdjItem, sizeof(FAdjItem));
}
