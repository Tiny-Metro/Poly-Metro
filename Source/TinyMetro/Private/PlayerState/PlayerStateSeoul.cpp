// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerState/PlayerStateSeoul.h"

APlayerStateSeoul::APlayerStateSeoul() : Super() {
	Money = 200;
	ValidItem.Lane = 3;
	ValidItem.Train = 3;
	ValidItem.Tunnel = 2;
	ValidItem.Bridge = 3;
}