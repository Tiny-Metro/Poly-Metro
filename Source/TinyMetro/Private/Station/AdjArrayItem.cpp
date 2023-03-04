// Fill out your copyright notice in the Description page of Project Settings.


#include "Station/AdjArrayItem.h"

void UAdjArrayItem::Add(FAdjItem Item, float Length)
{
	length.Add(Item, Length);
}

void UAdjArrayItem::RemoveRef(FAdjItem Item)
{
	length.Remove(Item);
}

int32 UAdjArrayItem::Num()
{
	return length.Num();
}
